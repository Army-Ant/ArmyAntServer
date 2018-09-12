#include <EventManager.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <UserSessionManager.h>

namespace ArmyAntServer{

LocalEventData::LocalEventData(int32 code) : code(code){}

LocalEventData::~LocalEventData(){}


EventManager::EventManager(UserSessionManager& sessionMgr):sessionMgr(sessionMgr){

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
	if(userses == nullptr)
		return false;
	auto finded = userses->localEventListenerList.find(code);
	if(finded == userses->localEventListenerList.end())
		return false;
	finded->second(data);
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
	if(userses == nullptr)
		return false;
	auto finded = userses->networkListenerList.find(code);
	if(finded == userses->networkListenerList.end())
		return false;
	// Find the conversation record
	// TODO : Have not been completed
	auto convRecord = userses->conversationWaitingList.find(conversationCode);
	switch(conversationStepType){
		case ArmyAntMessage::System::ConversationStepType::NoticeOnly:
			break;
		case ArmyAntMessage::System::ConversationStepType::AskFor:
			break;
		case ArmyAntMessage::System::ConversationStepType::StartConversation:
			break;
		case ArmyAntMessage::System::ConversationStepType::ConversationStepOn:
			break;
		case ArmyAntMessage::System::ConversationStepType::ResponseEnd:
			break;
		default:
			break;
	}

	// Call response listener
	finded->second(data);
	return true;
}

int32 EventManager::getProtobufMessageCode(google::protobuf::Message * message){
	return message->GetDescriptor()->options().GetExtension(msg_code);
}



} // namespace ArmyAntServer
