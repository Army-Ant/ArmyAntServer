#ifndef HUOLONG_USER_H_20181017
#define HUOLONG_USER_H_20181017

#include <AADefine.h>

namespace ArmyAntServer{

class HuolongUser{
public:
	HuolongUser(const char* userId, int32 clientId);
	~HuolongUser();

public:
	int32 getClientId()const;


private:
	AA_FORBID_ASSGN_OPR(HuolongUser);
	AA_FORBID_COPY_CTOR(HuolongUser);
};

}

#endif // HUOLONG_USER_H_20181017
