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

bool EventManager::removeListenerForLocalEvent(int32 code, int32 userId){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	auto finded = userses->localEventListenerList.find(code);
	if(finded != userses->localEventListenerList.end())
		userses->localEventListenerList.erase(finded);
	return true;
}

bool EventManager::dispatchLocalEvent(int32 code, int32 userId, LocalEventData*data){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Warning, LOGGER_TAG);
		return false;
	}
	auto finded = userses->localEventListenerList.find(code);
	if(finded == userses->localEventListenerList.end()){
		logger.pushLog("Cannot find the target listener when dispatching local event, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Import, LOGGER_TAG);
		return false;
	}
	userses->dispatchLocalEvent(code, data);
	return true;
}

bool EventManager::addListenerForNetworkResponse(int32 code, int32 userId, NetworkResponseCallback cb){
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr)
		return false;
	userses->networkListenerList.insert(std::make_pair(code, cb));
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

bool EventManager::dispatchNetworkResponse(int32 code, int32 userId, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message*data){
	// Find user and listener-handler
	auto userses = sessionMgr.getUserSession(userId);
	if(userses == nullptr){
		logger.pushLog("Cannot find the target user when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Import, LOGGER_TAG);
		return false;
	}
	auto finded = userses->networkListenerList.find(code);
	if(finded == userses->networkListenerList.end()){
		logger.pushLog("Cannot find the target listener when dispatching network message, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code), Logger::AlertLevel::Info, LOGGER_TAG);
		return false;
	}
	// Find the conversation record
	auto convRecord = userses->conversationWaitingList.find(conversationCode);
	switch(conversationStepType){
		case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
			if(convRecord != userses->conversationWaitingList.end()){
				logger.pushLog("Notice should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else if(conversationStepIndex != 0){
				logger.pushLog("Wrong waiting step index for notice, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			}
			break;
		case ArmyAntMessage::System::ConversationStepType::AskFor:
			if(convRecord != userses->conversationWaitingList.end()){
				logger.pushLog("Ask-for should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else if(conversationStepIndex != 0){
				logger.pushLog("Wrong waiting step index for ask-for, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else{
				// Record the ask-for
				userses->conversationWaitingList.insert(std::make_pair(conversationCode, 0));
			}
			break;
		case ArmyAntMessage::System::ConversationStepType::StartConversation:
			if(convRecord != userses->conversationWaitingList.end()){
				logger.pushLog("Conversation-start should not has the same code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else if(conversationStepIndex != 0){
				logger.pushLog("Wrong waiting step index for conversation-start, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else{
				// Record the conversation
				userses->conversationWaitingList.insert(std::make_pair(conversationCode, 1));
			}
			break;
		case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
			if(convRecord == userses->conversationWaitingList.end()){
				logger.pushLog("Conversation-step should has the past data code in waiting list, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation code: " + int64(conversationCode), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else if(convRecord->second != conversationStepIndex){
				logger.pushLog("Wrong waiting step index for conversation-step, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
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
				return false;
			} else if(convRecord->second != conversationStepIndex && convRecord->second != 0){
				logger.pushLog("Wrong waiting step index for end, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation step: " + int64(conversationStepIndex), Logger::AlertLevel::Warning, LOGGER_TAG);
				return false;
			} else{
				// Remove the waiting data
				userses->conversationWaitingList.erase(convRecord);
			}
			break;
		default:
			logger.pushLog("Network message unknown type number, user: " + ArmyAnt::String(userId) + " , message code: " + int64(code) + " , conversation type: " + int64(conversationStepType), Logger::AlertLevel::Warning, LOGGER_TAG);
			return false;
	}

	// Call response listener
	userses->dispatchNetworkEvent(code, data);
	return true;
}

int32 EventManager::getProtobufMessageCode(google::protobuf::Message * message){
	return message->GetDescriptor()->options().GetExtension(msg_code);
}



} // namespace ArmyAntServer
