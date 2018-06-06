#ifndef EVENT_MANAGER_H_20180606
#define EVENT_MANAGER_H_20180606

#include <AADefine.h>

namespace ArmyAntServer{

class EventManager{
public:
	EventManager();
	~EventManager();

public:
	void setEnable(bool enable);
	bool isEnabled()const;

	bool registerLocalEvent();
	bool dispatchLocalEvent();

	bool registerNetworkResponse();
	bool dispatchNetworkResponse();
	template<class T> inline T makeNetworkRequest();

private:
	AA_FORBID_ASSGN_OPR(EventManager);
	AA_FORBID_COPY_CTOR(EventManager);
};

template<class T>
inline T EventManager::makeNetworkRequest(){

	return false;
}

} // namespace ArmyAntServer

#endif // EVENT_MANAGER_H_20180606
