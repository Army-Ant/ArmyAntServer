#ifndef ECHO_APP_H_20180913
#define ECHO_APP_H_20180913

#include <map>
#include <memory>
#include <subApplication.h>

namespace ArmyAntServer{

class EchoApp : public SubApplication{
public:
	EchoApp(int64 appid, ServerMain&server);
	virtual ~EchoApp();

public:
	virtual bool onStart() override;
	virtual bool onStop() override;

	void onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);
	void onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);
	void onUserSend(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);
	void onUserBroadcast(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);

private:
	std::map<std::string, int32> userList;

private:
	AA_FORBID_ASSGN_OPR(EchoApp);
	AA_FORBID_COPY_CTOR(EchoApp);
};

}

#endif // ECHO_APP_H_20180913
