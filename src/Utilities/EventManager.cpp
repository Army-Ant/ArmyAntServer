#include <EventManager.h>
#include <AAString.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <UserSessionManager.h>
#include <Logger.h>

namespace ArmyAntServer{
static const char* const LOGGER_TAG = "EventManager";

LocalEventData::LocalEventData(int32 code) : code(code){}

LocalEventData::~LocalEventData(){}


EventManager::EventManager(UserSessionManager& sessionMgr, Logger&logger) : sessionMgr(sessionMgr), logger(logger){

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
		logger.pushLog("Cannot find the target user when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Warning, LOGGER_TAG);
		ret = false;
	} else{
		auto finded = userses->localEventListenerList.find(code);
		if(finded == userses->localEventListenerList.end()){
			logger.pushLog("Cannot find the target listener when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Import, LOGGER_TAG);
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

bool EventManager::dispatchNetworkResponse(int32 code, int32 userId, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*message, int32 messageLen){
	// Find user and listener-handler
	auto userses = sessionMgr.getUserSession(userId);
	bool ret = true;
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Import, LOGGER_TAG);
		ret = false;
	} else{
		auto finded = userses->networkListenerList.find(code);
		if(finded == userses->networkListenerList.end()){
			logger.pushLog("Cannot find the target listener when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Info, LOGGER_TAG);
			ret = false;
		}
		// Find the conversation record
		userses->ioMutex.lock();
		auto convRecord = userses->conversationWaitingList.find(conversationCode);
		switch(conversationStepType){
			case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
				if(convRecord != userses->conversationWaitingList.end()){
					logger.pushLog("Notice should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for notice, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::AskFor:
				if(convRecord != userses->conversationWaitingList.end()){
					logger.pushLog("Ask-for should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for ask-for, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the ask-for
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, 0));
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::StartConversation:
				if(convRecord != userses->conversationWaitingList.end()){
					logger.pushLog("Conversation-start should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(conversationStepIndex != 0){
					logger.pushLog("Wrong waiting step index for conversation-start, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the conversation
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, 1));
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
				if(convRecord == userses->conversationWaitingList.end()){
					logger.pushLog("Conversation-step should has the past data code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(convRecord->second != conversationStepIndex){
					logger.pushLog("Wrong waiting step index for conversation-step, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Record the conversation step
					userses->conversationWaitingList.erase(convRecord);
					userses->conversationWaitingList.insert(std::make_pair(conversationCode, conversationStepIndex + 1));
					convRecord = userses->conversationWaitingList.find(conversationCode);
				}
				break;
			case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
				if(convRecord == userses->conversationWaitingList.end()){
					logger.pushLog("The end should has the past data code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else if(convRecord->second != conversationStepIndex && convRecord->second != 0){
					logger.pushLog("Wrong waiting step index for end, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
					ret = false;
				} else{
					// Remove the waiting data
					userses->conversationWaitingList.erase(convRecord);
				}
				break;
			default:
				logger.pushLog("Network message unknown type number, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation type: " + int64(conversationStepType), Logger::AlertLevel::Warning, LOGGER_TAG);
				ret = false;
		}

		// Call response listener
		userses->ioMutex.unlock();
		userses->dispatchNetworkEvent(conversationCode, conversationCode, code, message, messageLen);
	}

	auto finded = networkListenerList.find(code);
	if(finded != networkListenerList.end()){
		for(auto i = finded->second.begin(); i != finded->second.end(); ++i){
			i->second(conversationCode, conversationCode, userId, message, messageLen);
		}
	}
	return ret;
}

int32 EventManager::getProtobufMessageCode(google::protobuf::Message * message){
	return message->GetDescriptor()->options().GetExtension(msg_code);
}



} // namespace ArmyAntServer
