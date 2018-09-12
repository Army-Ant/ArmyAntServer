#include <UserSessionManager.h>
#include <MessageQueueManager.h>

namespace ArmyAntServer{

UserSessionManager::UserSessionManager(MessageQueueManager&queueMgr)
	:queueMgr(queueMgr){

}

UserSessionManager::~UserSessionManager(){}

UserSession*UserSessionManager::getUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded == sessionList.end())
		return nullptr;
	auto&ret = finded->second;
	return  ret;
}

UserSession* UserSessionManager::createUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded != sessionList.end())
		return nullptr;
	sessionList.insert(std::make_pair(userId, new UserSession(userId, *queueMgr.createQueue(userId), *this)));
	auto&ret = sessionList.find(userId)->second;
	return ret;
}

bool UserSessionManager::removeUserSession(int32 userId){
	auto finded = sessionList.find(userId);
	if(finded == sessionList.end())
		return false;
	delete finded->second;
	sessionList.erase(finded);
	return true;
}

}
