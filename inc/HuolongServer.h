#ifndef HUOLONG_SERVER_H_20181013
#define HUOLONG_SERVER_H_20181013

#include <map>
#include <memory>
#include <subApplication.h>
#include <HuolongUser.h>
#include <HuolongUserDataManager.h>

namespace ArmyAntServer{

class HuolongServer : public SubApplication{
public:
	HuolongServer(int64 appid, ServerMain&server);
	virtual ~HuolongServer();

public:
	virtual bool onStart() override;
	virtual bool onStop() override;

	void onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);
	void onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);

	void onUserDisconnected(int32 userId);

private:
	std::map<std::string /* userId */, HuolongUser* /* clientId */> userList;
	HuolongUserDataManager dataMgr;

private:
	AA_FORBID_ASSGN_OPR(HuolongServer);
	AA_FORBID_COPY_CTOR(HuolongServer);
};

}

#endif // HUOLONG_SERVER_H_20181013
