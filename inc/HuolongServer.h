#ifndef HUOLONG_SERVER_H_20181013
#define HUOLONG_SERVER_H_20181013

#include <map>
#include <memory>
#include <string>
#include <DebugMutex.h>
#include <subApplication.h>
#include <HuolongUser.h>
#include <HuolongUserDataManager.h>
#include <HuolongTableManager.h>
#include <EventManager.h>

namespace ArmyAnt{
    class Logger;
}

namespace ArmyAntServer{

class HuolongServer : public SubApplication{
public:
	HuolongServer(int64 appid, ServerMain&server);
	virtual ~HuolongServer();

public: // manager handlers
	HuolongUserDataManager&getUserDataManager();
	HuolongTableManager&getTableManager();
	ArmyAnt::Logger&getLogger();
	HuolongUser*getUserById(std::string uid);

public: // listeners
	virtual bool onStart() override;
	virtual bool onStop() override;

	void onUserLogin(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);
	void onUserLogout(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length);

	void onUserDisconnected(int32 userId);

public: // users listener register
	bool addUserListenerForNetworkResponse(int32 code, int32 userId, EventManager::NetworkResponseCallback cb);
	bool removeUserListenerForNetworkResponse(int32 code, int32 userId);

	bool sendMsgToUser(std::string uid, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content);
	bool sendMsgToUser(std::string uid, int32 conversationCode, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message * content);

public: // operations


private:
	Mutex userListMutex;
	std::map<std::string /* userId */, HuolongUser* /* clientId */> userList;
	HuolongUserDataManager dataMgr;
	HuolongTableManager tableMgr;

private:
	AA_FORBID_ASSGN_OPR(HuolongServer);
	AA_FORBID_COPY_CTOR(HuolongServer);
};

}

#endif // HUOLONG_SERVER_H_20181013
