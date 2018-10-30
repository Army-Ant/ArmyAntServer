#include <HuolongUserDataManager.h>
#include <AATimeUtilities.h>

namespace ArmyAntServer{

static const int32 MAX_AUTO_AUTH_NUM = 6;
static const int64 AUTO_AUTH_TIME_LENGTH = 1296000;

HuolongUserDataManager::~HuolongUserDataManager(){
	for(auto i = userData.begin(); i != userData.end(); ++i){
		delete i->second;
	}
}

std::string HuolongUserDataManager::getUserIdByAuth(const char * auth){
	auto currTime = ArmyAnt::TimeUtilities::getCurrentTime();
	for(auto i = userData.begin(); i != userData.end(); ++i){
		for(auto k = i->second->autoLoginAuth.begin(); k != i->second->autoLoginAuth.end(); ++k){
			if(currTime > k->second){
				k = i->second->autoLoginAuth.erase(k);
			}
			if(auth == k->first){
				return i->first;
			}
		}
	}
	return "";
}

std::string HuolongUserDataManager::getUserId(LoginType type, std::string id, std::string auth){
	for(auto i = userData.begin(); i != userData.end(); ++i){
		auto ret = i->second->thirdData.find(type);
		if(ret != i->second->thirdData.end() && ret->second.id == id){
			if(ret->second.auth != auth){
				ret->second.auth = auth;
			}
			return i->first;
		}
	}
	generateNewUser(id);
	auto i = userData.find(id);
	i->second->thirdData.insert(std::make_pair(type, ThirdPartyAccountAuthData{id, auth}));
	return id;
}

std::string HuolongUserDataManager::generateAutoLoginAuth(std::string id){
	auto i = userData.find(id);
	if(i == userData.end()){
		generateNewUser(id);
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
	return ret;
}

HuolongUserDataManager::HuolongUserDataManager(){

}

std::string HuolongUserDataManager::generateNewUser(std::string id){
	userData.insert(std::make_pair(id, new UserData{id, ""}));
	return id;
}

}

