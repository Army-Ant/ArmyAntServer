#ifndef HUOLONG_USER_DATA_MANAGER_H_20181017
#define HUOLONG_USER_DATA_MANAGER_H_20181017

#include <AADefine.h>
#include <ArmyAntMessage/Common/base.pb.h>

#include <map>
#include <string>

namespace ArmyAntServer{

class HuolongUserDataManager{
public:
	~HuolongUserDataManager();

public:
	std::string getUserIdByAuth(const char*auth);
	std::string getUserId(LoginType type, std::string id, std::string auth);
	std::string generateAutoLoginAuth(std::string id);

private:
	struct ThirdPartyAccountAuthData{
		std::string id;
		std::string auth;
	};
	struct UserData{
		std::string uid;
		std::string pwd;
		std::map<LoginType, ThirdPartyAccountAuthData> thirdData;
		std::map<std::string, int64> autoLoginAuth;
	};

private:
	std::map<std::string, UserData*> userData;

private:
	AA_FORBID_ASSGN_OPR(HuolongUserDataManager);
	AA_FORBID_COPY_CTOR(HuolongUserDataManager);
	HuolongUserDataManager();
	std::string generateNewUser(std::string id);
	friend class HuolongServer;
};

}

#endif // HUOLONG_USER_DATA_MANAGER_H_20181017
