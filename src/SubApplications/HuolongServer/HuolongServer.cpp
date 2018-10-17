#include <HuolongServer.h>
#include <functional>

#include <Logger.h>
#include <ServerMain.h>
#include <EventManager.h>
#include <UserSession.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>


namespace ArmyAntServer{

#define NETWORK_CALLBACK(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

static const char* const EVENT_TAG = "HuolongServer";
static const char* const LOGGER_TAG = "HuolongServer";

HuolongServer::HuolongServer(int64 appid, ServerMain & server) :SubApplication(appid, server), userList(), dataMgr(){}

HuolongServer::~HuolongServer(){}

bool HuolongServer::onStart(){
	userList.clear();
	server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>(), NETWORK_CALLBACK(HuolongServer::onUserLogin));
	server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLogoutRequest>(), NETWORK_CALLBACK(HuolongServer::onUserLogout));

	server.getLogger().pushLog("Huolong server started !", Logger::AlertLevel::Info, LOGGER_TAG);
	return true;
}

bool HuolongServer::onStop(){
	server.getEventManager().removeGlobalListenerForLocalEvent(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>());
	return false;
}

void HuolongServer::onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse login protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto oldUser = userList.find(msg.user_id());
	ArmyAntMessage::SubApps::SM2C_HuolongLoginResponse response;
	if(oldUser != userList.end()){
		response.set_result(3);
		response.set_message(std::string("The user of this name has logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Login successful !"));
		server.getEventManager().addListenerForUserDisconnect(userId, EVENT_TAG, std::bind(&HuolongServer::onUserDisconnected, this, std::placeholders::_1));
		userList.insert(std::make_pair(msg.user_id(), new HuolongUser(msg.user_id().c_str(), userId)));
		server.getLogger().pushLog(("User login: " + msg.user_id()).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void HuolongServer::onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongLogoutRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse logout protocol failure !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	std::string userIdStr = dataMgr.getUserIdByAuth(msg.auto_login_auth().c_str());
	auto oldUser = userList.find(userIdStr);
	ArmyAntMessage::SubApps::SM2C_HuolongLogoutResponse response;
	if(oldUser == userList.end()){
		response.set_result(3);
		response.set_message(std::string("The user of this name has not logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Logout successful !"));
		server.getLogger().pushLog(("User logout: " + userIdStr).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void HuolongServer::onUserDisconnected(int32 userId){
	std::string fromUser = "";
	bool founded = false;
	for(auto i = userList.begin(); i != userList.end(); ++i){
		if(i->second->getClientId() == userId){
			founded = true;
			fromUser = i->first.c_str();
			userList.erase(fromUser);
			break;
		}
	}
	if(founded){
		server.getLogger().pushLog(("User disconnected without logout, user name: " + fromUser).c_str(), Logger::AlertLevel::Info, LOGGER_TAG);
	} else{
		server.getLogger().pushLog(("Get an unknown user disconnected signal ! userId:" + ArmyAnt::String(userId)).c_str(), Logger::AlertLevel::Error, LOGGER_TAG);
	}
}



#undef NETWORK_CALLBACK

}

