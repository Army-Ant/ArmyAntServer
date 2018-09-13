#ifndef SUB_APPLICATION_MANAGER_H_20180913
#define SUB_APPLICATION_MANAGER_H_20180913

#include <AADefine.h>

namespace ArmyAntServer{
class Logger;

class SubApplicationManager{
public:
	SubApplicationManager(Logger&logger);
	~SubApplicationManager();

public:

private:

private:
	AA_FORBID_ASSGN_OPR(SubApplicationManager);
	AA_FORBID_COPY_CTOR(SubApplicationManager);
};

}

#endif // SUB_APPLICATION_MANAGER_H_20180913
