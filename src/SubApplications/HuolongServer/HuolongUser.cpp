#include <HuolongUser.h>
#include <HuolongUserDataManager.h>

namespace ArmyAntServer{

HuolongUser::HuolongUser(LoginType type, std::string id, std::string auth, int32 clientId, HuolongUserDataManager&dataMgr)
	: type(type), dataMgr(dataMgr), clientId(clientId), userId(dataMgr.getUserId(type, id, auth)){

}

HuolongUser::HuolongUser(std::string autoLoginAuth, int32 clientId, HuolongUserDataManager & dataMgr)
	: type(LoginType::Auto), dataMgr(dataMgr), clientId(clientId), userId(dataMgr.getUserIdByAuth(autoLoginAuth.c_str())){

}

HuolongUser::~HuolongUser(){

}

std::string HuolongUser::getUserId() const{
	return userId;
}

int32 HuolongUser::getClientId() const{
	return clientId;
}

std::string HuolongUser::generateAutoLoginAuth(){
	return dataMgr.generateAutoLoginAuth(userId);
}


}
