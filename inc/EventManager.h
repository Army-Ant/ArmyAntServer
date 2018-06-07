#ifndef EVENT_MANAGER_H_20180606
#define EVENT_MANAGER_H_20180606

#include <functional>
#include <map>

#include <google/protobuf/message.h>

#include <AADefine.h>

namespace ArmyAntServer{

// local event data must be inherited from this class
// network response will defaultly inherited from google::protobuf::message
class LocalEventData{
public:
	LocalEventData();
	~LocalEventData();
};

class EventManager{
public:
	typedef std::function<void(int32 code, int32 userIndex, LocalEventData*data)> LocalEventCallback;
	typedef std::function<void(int32 code, int32 userIndex, google::protobuf::Message*message)> NetworkResponseCallback;

public:
	EventManager();
	~EventManager();

public:
	void setEnable(bool enable);
	bool isEnabled()const;
	bool setLocalEventCallback(LocalEventCallback cb);
	bool setNetworkResponseCallback(NetworkResponseCallback cb);

	bool registerLocalEvent(int32 code, int32 userIndex);
	bool unregisterLocalEvent(int32 code, int32 userIndex);
	bool dispatchLocalEvent(int32 code, int32 userIndex, LocalEventData*data);

	bool registerNetworkResponse(int32 code, int32 userIndex);
	bool unregisterNetworkResponse(int32 code, int32 userIndex);
	bool dispatchNetworkResponse(int32 code, int32 userIndex, google::protobuf::Message*message);

private:
	bool enabled;
	LocalEventCallback localCB;
	NetworkResponseCallback netCB;
	std::map<int32, std::vector<int32>> localListener;
	std::map<int32, std::vector<int32>> networkListener;

private:
	AA_FORBID_ASSGN_OPR(EventManager);
	AA_FORBID_COPY_CTOR(EventManager);
};

} // namespace ArmyAntServer

#endif // EVENT_MANAGER_H_20180606
