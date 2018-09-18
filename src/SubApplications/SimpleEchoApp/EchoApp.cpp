#include <EchoApp.h>
#include <functional>

#include <Logger.h>
#include <ServerMain.h>
#include <EventManager.h>
#include <UserSession.h>
#include <ArmyAntMessage/SubApps/SimpleEcho.pb.h>

namespace ArmyAntServer{

#define NETWORK_CALLBACK(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

static const char* const EVENT_TAG = "EchoApp";
static const char* const LOGGER_TAG = "EchoApp";

EchoApp::EchoApp(int64 appid, ServerMain & server) :SubApplication(appid, server), userList(){}

EchoApp::~EchoApp(){}

bool EchoApp::onStart(){
	userList.clear();
	server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoLoginRequest>(), NETWORK_CALLBACK(EchoApp::onUserLogin));

	server.getLogger().pushLog("Simple echo server started !", Logger::AlertLevel::Info, LOGGER_TAG);
	return true;
}

bool EchoApp::onStop(){
	server.getEventManager().removeGlobalListenerForLocalEvent(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoLoginRequest>());
	return false;
}

void EchoApp::onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_EchoLoginRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse login protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto oldUser = userList.find(msg.user_name());
	ArmyAntMessage::SubApps::SM2C_EchoLoginResponse response;
	if(oldUser != userList.end()){
		response.set_result(3);
		response.set_message(std::string("The user of this name has logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Login successful !"));
		userList.insert(std::make_pair(msg.user_name(), userId));
		server.getEventManager().addListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoLogoutRequest>(), userId, NETWORK_CALLBACK(EchoApp::onUserLogout));
		server.getEventManager().addListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoSendRequest>(), userId, NETWORK_CALLBACK(EchoApp::onUserSend));
		server.getEventManager().addListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoBroadcastRequest>(), userId, NETWORK_CALLBACK(EchoApp::onUserBroadcast));
		server.getEventManager().addListenerForUserDisconnect(userId, EVENT_TAG, std::bind(&EchoApp::onUserDisconnected, this, std::placeholders::_1));
		server.getLogger().pushLog(("User login: " + msg.user_name()).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void EchoApp::onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_EchoLogoutRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse logout protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto oldUser = userList.find(msg.user_name());
	ArmyAntMessage::SubApps::SM2C_EchoLogoutResponse response;
	if(oldUser == userList.end()){
		response.set_result(3);
		response.set_message(std::string("The user of this name has not logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Logout successful !"));
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoLogoutRequest>(), userId);
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoSendRequest>(), userId);
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoBroadcastRequest>(), userId);
		userList.erase(oldUser);
		server.getLogger().pushLog(("User logout: " + msg.user_name()).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void EchoApp::onUserSend(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_EchoSendRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse send protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	const char* fromUser = nullptr;
	for(auto i = userList.begin(); i != userList.end(); ++i){
		if(i->second == userId){
			fromUser = i->first.c_str();
			break;
		}
	}
	auto tarUser = userList.find(msg.target());
	ArmyAntMessage::SubApps::SM2C_EchoSendResponse response;
	auto ret = new ArmyAntMessage::SubApps::C2SM_EchoSendRequest(msg);
	response.set_allocated_request(ret);
	if(tarUser == userList.end()){
		response.set_result(4);
		response.set_message(std::string("The user of the target name has not logged in"));
	} else if(fromUser == nullptr){
		response.set_result(3);
		response.set_message(std::string("You have not logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Send successful !"));
		ArmyAntMessage::SubApps::SM2C_EchoReceiveNotice notice;
		notice.set_is_boradcast(false);
		notice.set_from(fromUser);
		notice.set_message(msg.message());
		server.getUserSessionManager().getUserSession(tarUser->second)->sendNetwork(extendVerstion, appid, 0, ArmyAntMessage::System::ConversationStepType::NoticeOnly, &notice);
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void EchoApp::onUserBroadcast(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_EchoBroadcastRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse send protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	const char* fromUser = nullptr;
	for(auto i = userList.begin(); i != userList.end(); ++i){
		if(i->second == userId){
			fromUser = i->first.c_str();
			break;
		}
	}
	ArmyAntMessage::SubApps::SM2C_EchoBroadcastResponse response;
	auto ret = new ArmyAntMessage::SubApps::C2SM_EchoBroadcastRequest(msg);
	response.set_allocated_request(ret);
	if(fromUser == nullptr){
		response.set_result(3);
		response.set_message(std::string("You have not logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Send successful !"));
		ArmyAntMessage::SubApps::SM2C_EchoReceiveNotice notice;
		notice.set_is_boradcast(true);
		notice.set_from(fromUser);
		notice.set_message(msg.message());
		for(auto i = userList.begin(); i != userList.end(); ++i){
			server.getUserSessionManager().getUserSession(i->second)->sendNetwork(extendVerstion, appid, 0, ArmyAntMessage::System::ConversationStepType::NoticeOnly, &notice);
		}
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void EchoApp::onUserDisconnected(int32 userId){
	std::string fromUser = "";
	bool founded = false;
	for(auto i = userList.begin(); i != userList.end(); ++i){
		if(i->second == userId){
			founded = true;
			fromUser = i->first.c_str();
			userList.erase(fromUser);
			break;
		}
	}
	if(founded){
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoLogoutRequest>(), userId);
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoSendRequest>(), userId);
		server.getEventManager().removeListenerForNetworkResponse(EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_EchoBroadcastRequest>(), userId);
		server.getLogger().pushLog(("User disconnected without logout, user name: " + fromUser).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	} else{
		server.getLogger().pushLog(("Get an unknown user disconnected signal ! userId:" + ArmyAnt::String(userId)).c_str(), Logger::AlertLevel::Error, LOGGER_TAG);
	}
}

#undef NETQWORK_CALLBACK

}
