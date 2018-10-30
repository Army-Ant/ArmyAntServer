#ifndef USER_SESSION_MANAGER_H_20180607
#define USER_SESSION_MANAGER_H_20180607

#include <map>

#include <AADefine.h>
#include <AALog.h>

#include "UserSession.h"

namespace ArmyAntServer{
class MessageQueueManager;
class SocketApplication;
class EventManager;

class UserSessionManager{
public:
	UserSessionManager(MessageQueueManager&queueMgr, ArmyAnt::Logger& logger);
	~UserSessionManager();

public:
	UserSession * getUserSession(int32 userId);
	UserSession * createUserSession(int32 userId, SocketApplication&socketSender, int32 senderIndex);
	bool removeUserSession(int32 userId);

	void setEventManager(EventManager&eventMgr);
	EventManager&getEventManager()const;

public:
	ArmyAnt::Logger& logger;
	EventManager*eventMgr;

private:
	MessageQueueManager&queueMgr;
	std::map<int32, UserSession*> sessionList;

	AA_FORBID_ASSGN_OPR(UserSessionManager);
	AA_FORBID_COPY_CTOR(UserSessionManager);
};

}// namespace ArmyAntServer


#endif // USER_SESSION_MANAGER_H_20180607
