#include <UserSession.h>
#include <UserSessionManager.h>
#include <MessageQueue.h>
#include <UserSessionMsgCode.h>
#include <Logger.h>
#include <AAString.h>

namespace ArmyAntServer{

static const char* const LOGGER_TAG = "UserSession";

UserSession::UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr)
	:index(index), msgQueue(msgQueue), mgr(mgr), userdata(nullptr), threadEnd(false), updateThread(std::bind(&UserSession::onUpdate, this)){}

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
void UserSession::dispatchLocalEvent(int32 code, LocalEventData * data){
	msgQueue.pushMessage(Message{UserSessionMsgCode::LocalEventMessage, code, new LocalEventData(*data)});
}

void UserSession::dispatchNetworkEvent(int32 code, google::protobuf::Message * data){
	void* newBuf = new int8[data->ByteSize()];
	memcpy(newBuf, data, data->ByteSize());
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
						cb->second(static_cast<google::protobuf::Message*>(msg.pdata));
						delete[] msg.pdata;
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
