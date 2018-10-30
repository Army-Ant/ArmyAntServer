#include <SubApplicationManager.h>
#include <subApplication.h>
#include <AALog.h>

namespace ArmyAntServer{



SubApplicationManager::SubApplicationManager(ArmyAnt::Logger & logger) :logger(logger), appList(){}

SubApplicationManager::~SubApplicationManager(){}

bool SubApplicationManager::registerApplication(int64 appid, SubApplication * app){
	if(appList.find(appid) != appList.end())
		return false;
	appList.insert(std::make_pair(appid, app));
	return true;
}

SubApplication* SubApplicationManager::unregisterApplication(int64 appid){
	auto finded = appList.find(appid);
	if(finded == appList.end())
		return nullptr;
	auto ret = finded->second;
	appList.erase(finded);
	return ret;
}

SubApplication * SubApplicationManager::getApplication(int64 appid) const{
	auto ret = appList.find(appid);
	if(ret != appList.end())
		return nullptr;
	return ret->second;
}

bool SubApplicationManager::startAllApplications(){
	bool ret = true;
	for(auto i = appList.begin(); i != appList.end(); ++i){
		ret = i->second->onStart() & ret;
	}
	return ret;
}

void SubApplicationManager::stopAllApplications(){
	for(auto i = appList.begin(); i != appList.end(); ++i){
		i->second->onStop();
	}
}

}
