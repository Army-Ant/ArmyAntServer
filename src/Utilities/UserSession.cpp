#include <UserSession.h>
#include <UserSessionManager.h>
#include <MessageQueue.h>
#include <UserSessionMsgCode.h>
#include <Logger.h>
#include <SocketApplication.h>
#include <AAString.h>

namespace ArmyAntServer{

static const char* const LOGGER_TAG = "UserSession";

UserSession::UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr)
	:index(index), msgQueue(msgQueue), mgr(mgr), userdata(nullptr), threadEnd(false), updateThread(std::bind(&UserSession::onUpdate, this)), ioMutex(){}

UserSession::UserSession(UserSession&&moved)
	: index(moved.index), msgQueue(moved.msgQueue), mgr(moved.mgr), userdata(moved.userdata), threadEnd(false), updateThread(std::bind(&UserSession::onUpdate, this)){}

UserSession::~UserSession(){
	threadEnd = true;
	updateThread.join();
}

UserSessionManager&UserSession::getManager(){
	return mgr;
}

int32 UserSession::getUserIndex(){
	return index;
}

void * UserSession::getUserData() const{
	return userdata;
}

void UserSession::setUserData(void * data){
	userdata = data;
}
bool UserSession::sendNetwork(int32 extendVersion, uint64 appid, int32 conversationCode, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content){
	int32 conversationStepIndex = 0;
	bool ret = true;
	ioMutex.lock();
	auto lastConversation = conversationWaitingList.find(conversationCode);
	switch(conversationStepType){
		case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
		case ArmyAntMessage::System::ConversationStepType::AskFor:
		case ArmyAntMessage::System::ConversationStepType::StartConversation:
			conversationStepIndex = 0;
			if(lastConversation != conversationWaitingList.end()){
				mgr.logger.pushLog("Sending a network message as conversation start with an existed code: " + int64(conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
				ret = false;
			}
			break;
		case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
			if(lastConversation != conversationWaitingList.end() && lastConversation->second == 0){
				mgr.logger.pushLog("Sending a network message as asking reply with an existed normal conversation code: " + int64(conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
				ret = false;
			}
		case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
			if(lastConversation == conversationWaitingList.end()){
				mgr.logger.pushLog("Sending a network message as conversation reply with an unexisted code: " + int64(conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
				ret = false;
			} else{
				conversationStepIndex = lastConversation->second;
				if(conversationStepIndex == 0)
					conversationStepIndex = 1;
			}
			break;
		default:
			mgr.logger.pushLog("Unknown conversation step type when sending a network message: " + int64(conversationStepType), Logger::AlertLevel::Error, LOGGER_TAG);
			ret = false;
	}
	if(!ret){
		ioMutex.unlock();
		return false;
	}

	switch(extendVersion){
		case 1:
		{
			ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1 extend;
			extend.set_app_id(appid);
			extend.set_content_length(content->ByteSize());
			extend.set_message_code(EventManager::getProtobufMessageCode(content));
			extend.set_conversation_code(conversationCode);
			extend.set_conversation_step_index(conversationStepIndex);
			extend.set_conversation_step_type(conversationStepType);
			mgr.socket.send(index, 0, MessageType::Normal, extendVersion, extend, content);
		}
		default:
			mgr.logger.pushLog("Sending a network message with an unknown head version: " + int64(extendVersion), Logger::AlertLevel::Error, LOGGER_TAG);
			ret = false;
	}

	switch(conversationStepType){
		case ArmyAntMessage::System::ConversationStepType::AskFor:
			conversationWaitingList.insert(std::make_pair(conversationCode, 0));
			break;
		case ArmyAntMessage::System::ConversationStepType::StartConversation:
			conversationWaitingList.insert(std::make_pair(conversationCode, 1));
			break;
		case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
		{
			auto inserting = std::make_pair(lastConversation->first, lastConversation->second + 1);
			conversationWaitingList.erase(lastConversation);
			conversationWaitingList.insert(inserting);
			break;
		}
		case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
			conversationWaitingList.erase(lastConversation);
			break;
	}

	ioMutex.unlock();
	return ret;
}
void UserSession::dispatchLocalEvent(int32 code, LocalEventData * data){
	msgQueue.pushMessage(Message{UserSessionMsgCode::LocalEventMessage, code, new LocalEventData(*data)});
}

void UserSession::dispatchNetworkEvent(int32 code, google::protobuf::Message * data){
	void* newBuf = new int8[data->ByteSize()];
	data->SerializePartialToArray(newBuf, data->ByteSize());
	msgQueue.pushMessage(Message{UserSessionMsgCode::NetworkEventMessage, code, newBuf});
}

void UserSession::onUpdate(){
	while(true){
		if(msgQueue.hasMessage()){
			auto msg = msgQueue.popMessage();
			switch(msg.id){
				case UserSessionMsgCode::LocalEventMessage:
				{
					auto cb = localEventListenerList.find(msg.data);
					if(cb != localEventListenerList.end()){
						LocalEventData* eventData = static_cast<LocalEventData*>(msg.pdata);
						cb->second(eventData);
						delete eventData;
					} else{
						mgr.logger.pushLog("Cannot find the listener when dispatching local event, code: " + ArmyAnt::String(msg.data) + " , user index: " + int64(index), Logger::AlertLevel::Warning, LOGGER_TAG);
					}
					break;
				}
				case UserSessionMsgCode::NetworkEventMessage:
				{
					auto cb = networkListenerList.find(msg.data);
					if(cb != networkListenerList.end()){
					} else{
						mgr.logger.pushLog("Cannot find the listener when dispatching network event, code: " + ArmyAnt::String(msg.data) + " , user index: " + int64(index), Logger::AlertLevel::Warning, LOGGER_TAG);
					}
					break;
				}
				default:
					mgr.logger.pushLog("Cannot find the event type when dispatching message, id: " + ArmyAnt::String(msg.id) + " , user index: " + int64(index), Logger::AlertLevel::Warning, LOGGER_TAG);
					break;
			}
		} else if(threadEnd){
			break;
		} else{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
}

}
