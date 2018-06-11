#ifndef USER_SESSION_MANAGER_H_20180607
#define USER_SESSION_MANAGER_H_20180607

#include <map>

#include <AADefine.h>

#include "UserSession.h"

namespace ArmyAntServer{
class ServerMain;

class UserSessionManager{
public:
	UserSessionManager(ServerMain&server);
	~UserSessionManager();

public:
	UserSession * getUserSession(int32 userId);
	UserSession * createUserSession(int32 userId);
	bool removeUserSession(int32 userId);

private:
	ServerMain & server;
	std::map<int32, UserSession> sessionList;

	AA_FORBID_ASSGN_OPR(UserSessionManager);
	AA_FORBID_COPY_CTOR(UserSessionManager);
};

}// namespace ArmyAntServer


#endif // USER_SESSION_MANAGER_H_20180607
