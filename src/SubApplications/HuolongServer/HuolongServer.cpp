#include <HuolongServer.h>
#include <functional>

#include <AALog.h>
#include <ServerMain.h>
#include <EventManager.h>
#include <UserSession.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>


namespace ArmyAntServer{

#define NETWORK_CALLBACK(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

static const char* const EVENT_TAG = "HuolongServer";
static const char* const LOGGER_TAG = "HuolongServer";

HuolongServer::HuolongServer(int64 appid, ServerMain & server) :SubApplication(appid, server), userListMutex(), userList(), dataMgr(*this), tableMgr(*this){}

HuolongServer::~HuolongServer(){}

HuolongUserDataManager&HuolongServer::getUserDataManager(){
	return dataMgr;
}

HuolongTableManager & HuolongServer::getTableManager(){
	return tableMgr;
}

ArmyAnt::Logger & HuolongServer::getLogger(){
	return server.getLogger();
}

HuolongUser * HuolongServer::getUserById(std::string uid){
	userListMutex.lock();
	auto oldUser = userList.find(uid);
	HuolongUser* ret = nullptr;
	if(oldUser != userList.end())
		ret = oldUser->second;
	userListMutex.unlock();
	return ret;
}

bool HuolongServer::onStart(){
	userListMutex.lock();
	userList.clear();
	userListMutex.unlock();
	server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>(), NETWORK_CALLBACK(HuolongServer::onUserLogin));
	server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLogoutRequest>(), NETWORK_CALLBACK(HuolongServer::onUserLogout));

	server.getLogger().pushLog("Huolong server started !", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	return true;
}

bool HuolongServer::onStop(){
	server.getEventManager().removeGlobalListenerForLocalEvent(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>());
	server.getEventManager().removeGlobalListenerForLocalEvent(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLogoutRequest>());
	return false;
}

void HuolongServer::onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse login protocol failure !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	HuolongUser* user = nullptr;
	if(msg.type() == LoginType::Auto){
		auto uid = dataMgr.getUserIdByAuth(msg.auto_login_auth().c_str());
		if(uid != "")
			user = new HuolongUser(*this, msg.auto_login_auth(), userId, dataMgr);
	} else{
		user = new HuolongUser(*this, msg.type(), msg.user_id().c_str(), msg.account_auth(), userId, dataMgr);
	}

	ArmyAntMessage::SubApps::SM2C_HuolongLoginResponse response;
	if(user == nullptr){  //  自动登录无效
		response.set_result(2);
		response.set_message(std::string("Invalid auto login authorication, maybe it was out of date"));
	} else{
		user->extendVerstion = extendVerstion;
		user->conversationCode = conversationCode;
		userListMutex.lock();
		auto oldUser = userList.find(user->getUserId());
		if(oldUser != userList.end()){  // 用户已登录
			response.set_result(3);
			response.set_message(std::string("The user of this id has logged in"));
		} else{ // 登录成功
			response.set_result(0);
			response.set_message(std::string("Login successful !"));
			server.getEventManager().addListenerForUserDisconnect(userId, EVENT_TAG, std::bind(&HuolongServer::onUserDisconnected, this, std::placeholders::_1));
			response.set_user_uid(user->getUserId());
			response.set_auto_login_auth(user->generateAutoLoginAuth());
			userList.insert(std::make_pair(user->getUserId(), user));
			server.getLogger().pushLog(("User login: " + user->getUserId()).c_str(), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
		}
		userListMutex.unlock();
	}
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void HuolongServer::onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void * message, int32 length){
	ArmyAntMessage::SubApps::C2SM_HuolongLogoutRequest msg;
	if(!msg.ParseFromArray(message, length)){
		server.getLogger().pushLog("Parse logout protocol failure !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	auto userSes = server.getUserSessionManager().getUserSession(userId);
	if(userSes == nullptr){
		server.getLogger().pushLog("Cannot find the request user in system session list !", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return;
	}
	std::string userIdStr = dataMgr.getUserIdByAuth(msg.auto_login_auth().c_str());
	userListMutex.lock();
	auto oldUser = userList.find(userIdStr);
	ArmyAntMessage::SubApps::SM2C_HuolongLogoutResponse response;
	if(oldUser == userList.end()){
		response.set_result(3);
		response.set_message(std::string("The user of this auth has not logged in"));
	} else{
		response.set_result(0);
		response.set_message(std::string("Logout successful !"));
		auto user = oldUser->second;
		userList.erase(oldUser);
		delete user;
		server.getLogger().pushLog(("User logout: " + userIdStr).c_str(), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	}
	userListMutex.unlock();
	userSes->sendNetwork(extendVerstion, appid, conversationCode, ArmyAntMessage::System::ConversationStepType::ResponseEnd, &response);
}

void HuolongServer::onUserDisconnected(int32 userId){
	std::string fromUser = "";
	bool founded = false;
	userListMutex.lock();
	for(auto i = userList.begin(); i != userList.end(); ++i){
		if(i->second->getClientId() == userId){
			founded = true;
			fromUser = i->first.c_str();
			auto oldUser = userList.find(fromUser);
			auto user = oldUser->second;
			userList.erase(oldUser);
			delete user;
			break;
		}
	}
	userListMutex.unlock();
	if(founded){
		server.getLogger().pushLog(("User disconnected without logout, user name: " + fromUser).c_str(), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	} else{
		server.getLogger().pushLog(("Get an unknown user disconnected signal ! userId:" + ArmyAnt::String(userId)).c_str(), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
	}
}

bool HuolongServer::addUserListenerForNetworkResponse(int32 code, int32 userId, EventManager::NetworkResponseCallback cb){
	return server.getEventManager().addListenerForNetworkResponse(code, userId, cb);
}

bool HuolongServer::removeUserListenerForNetworkResponse(int32 code, int32 userId){
	return server.getEventManager().removeListenerForNetworkResponse(code, userId);
}

bool HuolongServer::sendMsgToUser(std::string uid, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content){
	userListMutex.lock();
	auto user = userList.find(uid);
	auto has = user != userList.end();
	userListMutex.unlock();
	if(!has){
		server.getLogger().pushLog(("Cannot find the huolong user that will send message to ! userId:" + uid).c_str(), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return false;
	}
	auto userSes = server.getUserSessionManager().getUserSession(user->second->getClientId());
	if(!has){
		server.getLogger().pushLog(("Cannot find the huolong user's session that will send message to ! userId:" + uid + "sessionId: " + ArmyAnt::String(int64(user->second->getClientId())).c_str()).c_str(), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return false;
	}
	userSes->sendNetwork(user->second->extendVerstion, appid, ++user->second->conversationCode, conversationStepType, content);
	return true;
}

bool HuolongServer::sendMsgToUser(std::string uid, int32 conversationCode, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content){
	userListMutex.lock();
	auto user = userList.find(uid);
	auto has = user != userList.end();
	userListMutex.unlock();
	if(!has){
		server.getLogger().pushLog(("Cannot find the huolong user that will send message to ! userId:" + uid).c_str(), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return false;
	}
	auto userSes = server.getUserSessionManager().getUserSession(user->second->getClientId());
	if(!has){
		server.getLogger().pushLog(("Cannot find the huolong user's session that will send message to ! userId:" + uid + "sessionId: " + ArmyAnt::String(int64(user->second->getClientId())).c_str()).c_str(), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		return false;
	}
	userSes->sendNetwork(user->second->extendVerstion, appid, conversationCode, conversationStepType, content);
	return true;
}


#undef NETWORK_CALLBACK

}

