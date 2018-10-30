#ifndef SUB_APPLICATION_MANAGER_H_20180913
#define SUB_APPLICATION_MANAGER_H_20180913

#include <AADefine.h>
#include <AALog.h>
#include <map>

namespace ArmyAntServer{
class SubApplication;

class SubApplicationManager{
public:
	SubApplicationManager(ArmyAnt::Logger&logger);
	~SubApplicationManager();

public:
	bool registerApplication(int64 appid, SubApplication*app);
	SubApplication* unregisterApplication(int64 appid);
	SubApplication*getApplication(int64 appid)const;
	bool startAllApplications();
	void stopAllApplications();

private:
	ArmyAnt::Logger&logger;
	std::map<int64, SubApplication*> appList;

private:
	AA_FORBID_ASSGN_OPR(SubApplicationManager);
	AA_FORBID_COPY_CTOR(SubApplicationManager);
};

}

#endif // SUB_APPLICATION_MANAGER_H_20180913
