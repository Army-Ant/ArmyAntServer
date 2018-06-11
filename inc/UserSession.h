#ifndef USER_SESSION_H_20180607
#define USER_SESSION_H_20180607

#include <AADefine.h>

namespace ArmyAntServer{
class UserSessionManager;
class MessageQueue;

class UserSession{
public:
	UserSession(UserSession&&moved);
	~UserSession();

public:
	UserSessionManager & getManager();
	int32 getUserIndex();

private:
	int32 index;
	MessageQueue&msgQueue;
	UserSessionManager&mgr;

	UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr);
	AA_FORBID_ASSGN_OPR(UserSession);
	AA_FORBID_COPY_CTOR(UserSession);
	friend class UserSessionManager;
};

}// namespace ArmyAntServer


#endif // USER_SESSION_H_20180607
