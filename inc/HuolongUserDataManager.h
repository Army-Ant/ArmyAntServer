#ifndef HUOLONG_USER_DATA_MANAGER_H_20181017
#define HUOLONG_USER_DATA_MANAGER_H_20181017

#include <AADefine.h>
#include <DebugMutex.h>
#include <ArmyAntMessage/Common/base.pb.h>
#include <ArmyAntMessage/SubApps/huolong.pb.h>

#include <map>
#include <string>

namespace ArmyAntServer{
class HuolongServer;

class HuolongUserDataManager{
public:
	~HuolongUserDataManager();

public:
	std::string getUserIdByAuth(const char*auth);
	std::string getUserId(LoginType type, std::string id, std::string auth);
	std::string generateAutoLoginAuth(std::string id);

	ArmyAntMessage::SubApps::ST_HuolongPlayerInfo* getPlayerInfoMsg(std::string id);

public:
	bool setUserPasswordMd5(std::string id, std::string passwordMd5);
	std::string getUserPasswordMd5(std::string id);
	bool setUserNickname(std::string id, std::string nickname);
	std::string getUserNickname(std::string id);
	bool setUserAvatar(std::string id, std::string avatarUrl);
	std::string getUserAvatar(std::string id);

private:
	struct ThirdPartyAccountAuthData{
		std::string id;
		std::string auth;
	};
	struct UserData{
		std::string uid;
		std::string pwd;
		std::string nickname;
		std::string avatarUrl;
		std::map<LoginType, ThirdPartyAccountAuthData> thirdData;
		std::map<std::string, int64> autoLoginAuth;
	};

private:
	HuolongServer&hserver;
	Mutex mutex;
	std::map<std::string, UserData*> userData;

private:
	AA_FORBID_ASSGN_OPR(HuolongUserDataManager);
	AA_FORBID_COPY_CTOR(HuolongUserDataManager);
	HuolongUserDataManager(HuolongServer&hserver);
	std::string generateNewUser(std::string id);
	friend class HuolongServer;
};

}

#endif // HUOLONG_USER_DATA_MANAGER_H_20181017
