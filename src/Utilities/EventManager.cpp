#include <EventManager.h>
#include <AAString.h>
#include <AALog.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <UserSessionManager.h>

namespace ArmyAntServer{
static const char* const LOGGER_TAG = "EventManager";

LocalEventData::LocalEventData(int32 code) : code(code){}

LocalEventData::~LocalEventData(){}


EventManager::EventManager(UserSessionManager& sessionMgr, ArmyAnt::Logger&logger) : sessionMgr(sessionMgr), logger(logger){

}

EventManager::~EventManager(){

}

bool EventManager::addListenerForLocalEvent(int32 code, int32 userId, LocalEventCallback cb){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	userses->localEventListenerList.insert(std::make_pair(code, cb));
	return true;
}

bool EventManager::addGlobalListenerForLocalEvent(std::string tag, int32 code, LocalEventCallback cb){
	auto finded = localEventListenerList.find(code);
	if(finded == localEventListenerList.end()){
		localEventListenerList.insert(std::make_pair(code, std::map<std::string, LocalEventCallback>()));
		finded = localEventListenerList.find(code);
	}
	finded->second.insert(std::make_pair(tag, cb));
	return true;
}

bool EventManager::removeListenerForLocalEvent(int32 code, int32 userId){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	auto finded = userses->localEventListenerList.find(code);
	if(finded != userses->localEventListenerList.end())
		userses->localEventListenerList.erase(finded);
	return true;
}

bool EventManager::removeGlobalListenerForLocalEvent(std::string tag, int32 code){
	auto finded = localEventListenerList.find(code);
	if(finded == localEventListenerList.end()){
		return false;
	}
	auto findedInner = finded->second.find(tag);
	if(findedInner == finded->second.end()){
		return false;
	}
	finded->second.erase(findedInner);
	return true;
}

bool EventManager::dispatchLocalEvent(int32 code, int32 userId, LocalEventData*data){
	auto userses = sessionMgr.getUserSession(userId);
	bool ret = true;
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
		ret = false;
	} else{
		auto finded = userses->localEventListenerList.find(code);
		if(finded == userses->localEventListenerList.end()){
			ret = false;
		} else{
			userses->dispatchLocalEvent(code, data);
		}
	}
	auto finded = localEventListenerList.find(code);
	if(finded != localEventListenerList.end()){
		for(auto i = finded->second.begin(); i != finded->second.end(); ++i){
			i->second(userId, data);
		}
	}else if(!ret){
		logger.pushLog("Cannot find the target listener when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), ArmyAnt::Logger::AlertLevel::Import, LOGGER_TAG);
	}
	return ret;
}

bool EventManager::addListenerForNetworkResponse(int32 code, int32 userId, NetworkResponseCallback cb){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	userses->networkListenerList.insert(std::make_pair(code, cb));
	return true;
}

bool EventManager::addGlobalListenerForNetworkResponse(std::string tag, int32 code, NetworkResponseCallback cb){
	auto finded = networkListenerList.find(code);
	if(finded == networkListenerList.end()){
		networkListenerList.insert(std::make_pair(code, std::map<std::string, NetworkResponseCallback>()));
		finded = networkListenerList.find(code);
	}
	finded->second.insert(std::make_pair(tag, cb));
	return true;
}

bool EventManager::removeListenerForNetworkResponse(int32 code, int32 userId){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	auto finded = userses->networkListenerList.find(code);
	if(finded != userses->networkListenerList.end())
		userses->networkListenerList.erase(finded);
	return true;
}

bool EventManager::removeGlobalListenerForNetworkResponse(std::string tag, int32 code){
	auto finded = networkListenerList.find(code);
	if(finded == networkListenerList.end()){
		return false;
	}
	auto findedInner = finded->second.find(tag);
	if(findedInner == finded->second.end()){
		return false;
	}
	finded->second.erase(findedInner);
	return true;
}

bool EventManager::dispatchNetworkResponse(int32 extendVerstion, int32 code, int32 userId, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*message, int32 messageLen){
	// Find user and listener-handler
	auto userses = sessionMgr.getUserSession(userId);
	bool ret = true;
	bool noListener = false;
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), ArmyAnt::Logger::AlertLevel::Import, LOGGER_TAG);
		ret = false;
	} else{
		auto finded = userses->networkListenerList.find(code);
		if(finded == userses->networkListenerList.end()){
			ret = false;
			noListener = true;
		}
		// Find the conversation record
		userses->ioMutex.lock();
		bool isEnd = false;
		int32 cStepIndex = 0;
		{
			// 因为发生了死锁问题, 所以决定将数据取出来立即解锁, 以解决拖锁导致死锁的问题
			// 虽然看起来没什么用, 毕竟这里仅仅是检验和处理数据, 没有执行其他操作, 关键导致死锁的原因应该在usersession
			auto convRecord = userses->conversationWaitingList.find(conversationCode);
			isEnd = convRecord == userses->conversationWaitingList.end();
			if(!isEnd)
				cStepIndex = convRecord->second;
		}
		userses->ioMutex.unlock();
		switch(conversationStepType){
			case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
				if(!isEnd){
					logger.pushLog("Notice should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for notice, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::AskFor:
				if(!isEnd){
					logger.pushLog("Ask-for should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for ask-for, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the ask-for
					userses->ioMutex.lock();
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, 0));
					userses->ioMutex.unlock();
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::StartConversation:
				if(!isEnd){
					logger.pushLog("Conversation-start should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for conversation-start, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the conversation
					userses->ioMutex.lock();
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, 1));
					userses->ioMutex.unlock();
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
				if(isEnd){
					logger.pushLog("Conversation-step should has the past data code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(cStepIndex != conversationStepIndex){
					logger.pushLog("Wrong waiting step index for conversation-step, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the conversation step
					userses->ioMutex.lock();
					auto convRecord = userses->conversationWaitingList.find(conversationCode);
					userses->conversationWaitingList.erase(convRecord);
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, conversationStepIndex + 1));
					userses->ioMutex.unlock();
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
				if(isEnd){
					logger.pushLog("The end should has the past data code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(cStepIndex != conversationStepIndex && cStepIndex != 0){
					logger.pushLog("Wrong waiting step index for end, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Remove the waiting data
					userses->ioMutex.lock();
					auto convRecord = userses->conversationWaitingList.find(conversationCode);
					userses->conversationWaitingList.erase(convRecord);
					userses->ioMutex.unlock();
				}
				break;
			default:
				logger.pushLog("Network message unknown type number, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation type: " + int64(conversationStepType), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
				ret = false;
		}

		// Call response listener
		userses->dispatchNetworkEvent(extendVerstion, conversationCode, code, message, messageLen);
	}

	auto finded = networkListenerList.find(code);
	if(finded != networkListenerList.end()){
		for(auto i = finded->second.begin(); i != finded->second.end(); ++i){
			i->second(extendVerstion, conversationCode, userId, message, messageLen);
		}
	} else if(noListener){
		logger.pushLog("Cannot find the target listener when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	}
	return ret;
}

bool EventManager::addListenerForUserDisconnect(int32 userId, std::string tag, DisconnectCallback cb){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	auto finded = userses->disconnectCallback.find(tag);
	if(finded == userses->disconnectCallback.end()){
		userses->disconnectCallback.insert(std::make_pair(tag, cb));
		return true;
	}
	return false;
}

bool EventManager::addGlobalListenerForUserDisconnect(std::string tag, DisconnectCallback cb){
	auto finded = disconnectListenerList.find(tag);
	if(finded == disconnectListenerList.end()){
		disconnectListenerList.insert(std::make_pair(tag, cb));
		return true;
	}

	return false;
}

bool EventManager::removeListenerForUserDisconnect(int32 userId, std::string tag){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	auto finded = userses->disconnectCallback.find(tag);
	if(finded == userses->disconnectCallback.end()){
		return false;
	}
	userses->disconnectCallback.erase(finded);
	return true;
}

bool EventManager::removeGlobalListenerForUserDisconnect(std::string tag){
	auto finded = disconnectListenerList.find(tag);
	if(finded == disconnectListenerList.end()){
		return false;
	}
	disconnectListenerList.erase(finded);
	return true;
}

bool EventManager::dispatchUserDisconnected(int32 userId){
	auto userses = sessionMgr.getUserSession(userId);
	bool ret = true;
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching disconnect event, user: " + ArmyAnt::String(userId), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		ret = false;
	} else{
		for(auto i = userses->disconnectCallback.begin();i!= userses->disconnectCallback.end();++i)
			i->second(userId);
	}
	for(auto i = disconnectListenerList.begin(); i != disconnectListenerList.end(); ++i){
		i->second(userId);
	}
	return ret;
}

int32 EventManager::getProtobufMessageCode(google::protobuf::Message * message){
	return message->GetDescriptor()->options().GetExtension(msg_code);
}



} // namespace ArmyAntServer
