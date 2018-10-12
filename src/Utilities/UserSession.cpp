#include <UserSession.h>
#include <UserSessionManager.h>
#include <MessageQueue.h>
#include <MessageQueueManager.h>
#include <EventManager.h>
#include <UserSessionMsgCode.h>
#include <Logger.h>
#include <SocketApplication.h>
#include <AAString.h>

namespace ArmyAntServer{

static const char* const LOGGER_TAG = "UserSession";

struct NetworkEventStruct{
	int32 extendVersion;
	int32 conversationCode;
	int32 code;
	void * data;
	int32 length;
};
struct NetworkSendStruct{
	int32 extendVersion;
	int64 appid;
	int32 conversationCode;
	ArmyAntMessage::System::ConversationStepType conversationStepType;
	int32 code;
	void * data;
	int32 length;
};

UserSession::UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr, SocketApplication&socketSender, int32 senderIndex)
	:index(index), msgQueue(msgQueue), mgr(mgr),socketSender(socketSender), senderIndex(senderIndex), userdata(nullptr), threadEnd(false), updateThread(std::bind(&UserSession::onUpdate, this)), ioMutex(){}

UserSession::UserSession(UserSession&&moved)
	: index(moved.index), msgQueue(moved.msgQueue), mgr(moved.mgr), socketSender(moved.socketSender), senderIndex(moved.senderIndex), userdata(moved.userdata), threadEnd(false), updateThread(std::bind(&UserSession::onUpdate, this)){}

UserSession::~UserSession(){
	mgr.getEventManager().dispatchUserDisconnected(index);
	threadEnd = true;
	updateThread.join();
	msgQueue.getManager().removeQueue(index);
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
void UserSession::sendNetwork(int32 extendVersion, int64 appid, int32 conversationCode, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content){
	char* cpMsg = new char[content->ByteSize()];
	content->SerializePartialToArray(cpMsg, content->ByteSize());
	auto code = EventManager::getProtobufMessageCode(content);
	msgQueue.pushMessage(Message{UserSessionMsgCode::SendNetworkMessage, code, new NetworkSendStruct{ extendVersion, appid, conversationCode, conversationStepType, code, cpMsg, content->ByteSize() }});
}
void UserSession::dispatchLocalEvent(int32 code, LocalEventData * data){
	msgQueue.pushMessage(Message{UserSessionMsgCode::LocalEventMessage, code, new LocalEventData(*data)});
}

void UserSession::dispatchNetworkEvent(int32 extendVerstion, int32 conversationCode, int32 code, void * data, int32 length){
	void* newBuf = new int8[length];
	memcpy(newBuf, data, length);
	msgQueue.pushMessage(Message{UserSessionMsgCode::NetworkEventMessage, code, new NetworkEventStruct{extendVerstion, conversationCode, code, newBuf, length}});
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
						cb->second(index, eventData);
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
						auto evt = static_cast<NetworkEventStruct*>(msg.pdata);
						cb->second(evt->extendVersion, evt->conversationCode, index, evt->data, evt->length);
						ArmyAnt::Fragment::AA_SAFE_DELALL(evt->data);
						ArmyAnt::Fragment::AA_SAFE_DEL(evt);
					} else{
						mgr.logger.pushLog("Cannot find the listener when dispatching network event, code: " + ArmyAnt::String(msg.data) + " , user index: " + int64(index), Logger::AlertLevel::Warning, LOGGER_TAG);
					}
					break;
				}
				case UserSessionMsgCode::SendNetworkMessage:
				{
					auto evt = static_cast<NetworkSendStruct*>(msg.pdata);
					int32 conversationStepIndex = 0;
					bool ret = true;
					ioMutex.lock();
					auto lastConversation = conversationWaitingList.find(evt->conversationCode);
					switch(evt->conversationStepType){
						case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
						case ArmyAntMessage::System::ConversationStepType::AskFor:
						case ArmyAntMessage::System::ConversationStepType::StartConversation:
							conversationStepIndex = 0;
							if(lastConversation != conversationWaitingList.end()){
								mgr.logger.pushLog(ArmyAnt::String("Sending a network message as conversation start with an existed code: ") + int64(evt->conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
								ret = false;
							}
							break;
						case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
							if(lastConversation != conversationWaitingList.end() && lastConversation->second == 0){
								mgr.logger.pushLog(ArmyAnt::String("Sending a network message as asking reply with an existed normal conversation code: ") + int64(evt->conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
								ret = false;
							}
						case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
							if(lastConversation == conversationWaitingList.end()){
								mgr.logger.pushLog(ArmyAnt::String("Sending a network message as conversation reply with an unexisted code: ") + int64(evt->conversationCode), Logger::AlertLevel::Error, LOGGER_TAG);
								ret = false;
							} else{
								conversationStepIndex = lastConversation->second;
								if(conversationStepIndex == 0)
									conversationStepIndex = 1;
							}
							break;
						default:
							mgr.logger.pushLog(ArmyAnt::String("Unknown conversation step type when sending a network message: ") + int64(evt->conversationStepType), Logger::AlertLevel::Error, LOGGER_TAG);
							ret = false;
					}
					if(ret){
						switch(evt->extendVersion){
							case 1:
							{
								ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1 extend;
								extend.set_app_id(evt->appid);
								extend.set_content_length(evt->length);
								extend.set_message_code(evt->code);
								extend.set_conversation_code(evt->conversationCode);
								extend.set_conversation_step_index(conversationStepIndex);
								extend.set_conversation_step_type(evt->conversationStepType);
								socketSender.send(senderIndex, 0, MessageType::Normal, evt->extendVersion, extend, evt->data);
								break;
							}
							default:
								mgr.logger.pushLog(ArmyAnt::String("Sending a network message with an unknown head version: ") + int64(evt->extendVersion), Logger::AlertLevel::Error, LOGGER_TAG);
								ret = false;
						}

						switch(evt->conversationStepType){
							case ArmyAntMessage::System::ConversationStepType::AskFor:
								conversationWaitingList.insert(std::make_pair(evt->conversationCode, 0));
								break;
							case ArmyAntMessage::System::ConversationStepType::StartConversation:
								conversationWaitingList.insert(std::make_pair(evt->conversationCode, 1));
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
					}
					ioMutex.unlock();
					ArmyAnt::Fragment::AA_SAFE_DELALL(evt->data);
					ArmyAnt::Fragment::AA_SAFE_DEL(evt);
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
