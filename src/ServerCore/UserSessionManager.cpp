#include <UserSessionManager.h>
#include <ServerMain.h>
#include <MessageQueueManager.h>

namespace ArmyAntServer{

UserSessionManager::UserSessionManager(ServerMain&server)
	:server(server){

}

UserSessionManager::~UserSessionManager(){}

UserSession*UserSessionManager::getUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded == sessionList.end())
		return nullptr;
	auto&ret = finded->second;
	return  &ret;
}

UserSession* UserSessionManager::createUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded != sessionList.end())
		return nullptr;
	sessionList.insert(std::make_pair(userId, UserSession(userId, *server.getMessageQueueManager().createQueue(userId), *this)));
	auto&ret = sessionList.find(userId)->second;
	return &ret;
}

bool UserSessionManager::removeUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded == sessionList.end())
		return false;
	sessionList.erase(finded);
	return true;
}

}
