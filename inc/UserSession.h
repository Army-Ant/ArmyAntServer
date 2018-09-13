#ifndef USER_SESSION_H_20180607
#define USER_SESSION_H_20180607

#include <AADefine.h>
#include <map>
#include <functional>
#include <thread>
#include <EventManager.h>

namespace ArmyAntServer{
class UserSessionManager;
class MessageQueue;
class Logger;

class UserSession{
public:
	UserSession(UserSession&&moved);
	~UserSession();

public:    // Outer data
	UserSessionManager & getManager();
	int32 getUserIndex();
	void*getUserData()const;
	void setUserData(void*data);

public:
	void dispatchLocalEvent(int32 code, LocalEventData* data);
	void dispatchNetworkEvent(int32 code, google::protobuf::Message* data);

public:
	void onUpdate();

private:
	// Outer data
	int32 index;
	MessageQueue&msgQueue;
	UserSessionManager&mgr;
	void*userdata;

	bool threadEnd;
	std::thread updateThread;

	// Event data
	std::map<int32, EventManager::LocalEventCallback> localEventListenerList;
	std::map<int32, EventManager::NetworkResponseCallback> networkListenerList;
	std::map<int32, int32> conversationWaitingList;

	// unused private constructor
	UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr);
	AA_FORBID_ASSGN_OPR(UserSession);
	AA_FORBID_COPY_CTOR(UserSession);
	friend class UserSessionManager;
	friend class EventManager;
};

}// namespace ArmyAntServer


#endif // USER_SESSION_H_20180607
