#include <HuolongUserDataManager.h>
#include <AATimeUtilities.h>
#include <HuolongServer.h>

namespace ArmyAntServer{

static const int32 MAX_AUTO_AUTH_NUM = 6;
static const int64 AUTO_AUTH_TIME_LENGTH = 1296000;

HuolongUserDataManager::~HuolongUserDataManager(){
	mutex.lock();
	for(auto i = userData.begin(); i != userData.end(); ++i){
		delete i->second;
	}
	mutex.unlock();
}

std::string HuolongUserDataManager::getUserIdByAuth(const char * auth){
	auto currTime = ArmyAnt::TimeUtilities::getCurrentTime();
	mutex.lock();
	for(auto i = userData.begin(); i != userData.end(); ++i){
		for(auto k = i->second->autoLoginAuth.begin(); k != i->second->autoLoginAuth.end(); ++k){
			if(currTime > k->second){
				k = i->second->autoLoginAuth.erase(k);
			}
			if(auth == k->first){
				auto ret = i->first;
				mutex.unlock();
				return ret;
			}
		}
	}
	mutex.unlock();
	return "";
}

std::string HuolongUserDataManager::getUserId(LoginType type, std::string id, std::string auth){
	mutex.lock();
	for(auto i = userData.begin(); i != userData.end(); ++i){
		auto ret = i->second->thirdData.find(type);
		if(ret != i->second->thirdData.end() && ret->second.id == id){
			if(ret->second.auth != auth){
				ret->second.auth = auth;
			}
			auto ret = i->first;
			mutex.unlock();
			return ret;
		}
	}
	mutex.unlock();
	generateNewUser(id);
	mutex.lock();
	auto i = userData.find(id);
	i->second->thirdData.insert(std::make_pair(type, ThirdPartyAccountAuthData{id, auth}));
	mutex.unlock();
	return id;
}

std::string HuolongUserDataManager::generateAutoLoginAuth(std::string id){
	mutex.lock();
	auto i = userData.find(id);
	if(i == userData.end()){
		mutex.unlock();
		generateNewUser(id);
		mutex.lock();
		i = userData.find(id);
	}
	while(i->second->autoLoginAuth.size() > MAX_AUTO_AUTH_NUM){
		i->second->autoLoginAuth.erase(i->second->autoLoginAuth.begin());
	}
	std::string ret = id;
	auto timeStamp = ArmyAnt::TimeUtilities::getTimeStamp();
	ret += timeStamp.c_str();
	auto currTime = ArmyAnt::TimeUtilities::getCurrentTime();
	i->second->autoLoginAuth.insert(std::make_pair(ret, currTime + AUTO_AUTH_TIME_LENGTH));
	mutex.unlock();
	return ret;
}

ArmyAntMessage::SubApps::ST_HuolongPlayerInfo * HuolongUserDataManager::getPlayerInfoMsg(std::string id){
	mutex.lock();
	auto i = userData.find(id);
	if(i == userData.end()){
		mutex.unlock();
		return nullptr;
	}
	auto ret = new ArmyAntMessage::SubApps::ST_HuolongPlayerInfo();
	ret->set_user_uid(id.c_str());
	ret->set_nickname(i->second->nickname.c_str());
	ret->set_avatar_url(i->second->avatarUrl.c_str());
	mutex.unlock();
	return ret;
}

bool HuolongUserDataManager::setUserPasswordMd5(std::string id, std::string passwordMd5){
	mutex.lock();
	auto user = userData.find(id);
	if(user == userData.end()){
		mutex.unlock();
		return false;
	}
	user->second->pwd = passwordMd5;
	mutex.unlock();
	return true;
}

std::string HuolongUserDataManager::getUserPasswordMd5(std::string id){
	mutex.lock();
	auto user = userData.find(id);
	std::string ret = "";
	if(user != userData.end())
		ret = user->second->pwd;
	mutex.unlock();
	return ret;
}

bool HuolongUserDataManager::setUserNickname(std::string id, std::string nickname){
	mutex.lock();
	auto user = userData.find(id);
	if(user == userData.end()){
		mutex.unlock();
		return false;
	}
	user->second->nickname = nickname;
	mutex.unlock();
	return true;
}

std::string HuolongUserDataManager::getUserNickname(std::string id){
	mutex.lock();
	auto user = userData.find(id);
	std::string ret = "";
	if(user != userData.end())
		ret = user->second->nickname;
	mutex.unlock();
	return ret;
}

bool HuolongUserDataManager::setUserAvatar(std::string id, std::string avatarUrl){
	mutex.lock();
	auto user = userData.find(id);
	if(user == userData.end()){
		mutex.unlock();
		return false;
	}
	user->second->avatarUrl = avatarUrl;
	mutex.unlock();
	return true;
}

std::string HuolongUserDataManager::getUserAvatar(std::string id){
	mutex.lock();
	auto user = userData.find(id);
	std::string ret = "";
	if(user != userData.end())
		ret = user->second->avatarUrl;
	mutex.unlock();
	return ret;
}

HuolongUserDataManager::HuolongUserDataManager(HuolongServer&hserver):hserver(hserver), mutex(), userData(){

}

std::string HuolongUserDataManager::generateNewUser(std::string id){
	mutex.lock();
	userData.insert(std::make_pair(id, new UserData{id, "", "", ""}));
	mutex.unlock();
	return id;
}

}

