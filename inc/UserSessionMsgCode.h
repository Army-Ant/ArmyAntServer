#ifndef USER_SESSION_MSG_CODE_H_20180913
#define USER_SESSION_MSG_CODE_H_20180913

#include <AADefine.h>

namespace ArmyAntServer{

struct UserSessionMsgCode{
	static const int32 LocalEventMessage = 1;
	static const int32 NetworkEventMessage = 2;
	static const int32 SendNetworkMessage = 3;
};

}

#endif // USER_SESSION_MSG_CODE_H_20180913
