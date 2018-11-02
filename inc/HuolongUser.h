#ifndef HUOLONG_USER_H_20181017
#define HUOLONG_USER_H_20181017

#include <string>
#include <AADefine.h>
#include <ArmyAntMessage/Common/base.pb.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>

namespace ArmyAntServer{
class HuolongServer;
class HuolongUserDataManager;

class HuolongUser{
public:
	HuolongUser(HuolongServer& hserver, LoginType type, std::string id, std::string auth, int32 clientId, HuolongUserDataManager&dataMgr);
	HuolongUser(HuolongServer& hserver, std::string autoLoginAuth, int32 clientId, HuolongUserDataManager&dataMgr);
	~HuolongUser();

public:
	std::string getUserId() const;
	int32 getClientId()const;
	std::string generateAutoLoginAuth();

	bool setUserPasswordMd5(std::string passwordMd5);
	std::string getUserPasswordMd5();
	bool setUserNickname(std::string nickname);
	std::string getUserNickname();
	bool setUserAvatar(std::string avatarUrl);
	std::string getUserAvatar();

	ArmyAntMessage::SubApps::ST_HuolongPlayerInfo* getPlayerInfoMsg();

	void onCreateTable(int32, int32 convCode, int32, void * message, int32 length);
	void onEnterTable(int32, int32 convCode, int32, void * message, int32 length);

private:
	LoginType type;
	std::string userId;
	int32 clientId;

	HuolongServer& hserver;
	HuolongUserDataManager& dataMgr;

	int32 extendVerstion;
	int32 conversationCode;

private:
	AA_FORBID_ASSGN_OPR(HuolongUser);
	AA_FORBID_COPY_CTOR(HuolongUser);
	friend class HuolongServer;
};

}

#endif // HUOLONG_USER_H_20181017
