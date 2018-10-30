#ifndef HUOLONG_USER_H_20181017
#define HUOLONG_USER_H_20181017

#include <AADefine.h>
#include <ArmyAntMessage/Common/base.pb.h>
#include <string>

namespace ArmyAntServer{
class HuolongUserDataManager;

class HuolongUser{
public:
	HuolongUser(LoginType type, std::string id, std::string auth, int32 clientId, HuolongUserDataManager&dataMgr);
	HuolongUser(std::string autoLoginAuth, int32 clientId, HuolongUserDataManager&dataMgr);
	~HuolongUser();

public:
	std::string getUserId() const;
	int32 getClientId()const;
	std::string generateAutoLoginAuth();

private:
	LoginType type;
	std::string userId;
	int32 clientId;

	HuolongUserDataManager& dataMgr;

private:
	AA_FORBID_ASSGN_OPR(HuolongUser);
	AA_FORBID_COPY_CTOR(HuolongUser);
};

}

#endif // HUOLONG_USER_H_20181017
