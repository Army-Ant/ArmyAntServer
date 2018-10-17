#ifndef HUOLONG_USER_DATA_MANAGER_H_20181017
#define HUOLONG_USER_DATA_MANAGER_H_20181017

#include <AADefine.h>
#include <HuolongServer.h>

namespace ArmyAntServer{

class HuolongUserDataManager{
public:
	~HuolongUserDataManager();

public:
	const char*getUserIdByAuth(const char*auth);

private:
	AA_FORBID_ASSGN_OPR(HuolongUserDataManager);
	AA_FORBID_COPY_CTOR(HuolongUserDataManager);
	HuolongUserDataManager();
	friend class HuolongServer;
};

}

#endif // HUOLONG_USER_DATA_MANAGER_H_20181017
