#ifndef SERVER_MAIN_H_20180522
#define SERVER_MAIN_H_20180522

#include <map>
#include <thread>

#include <AADefine.h>

#include <Logger.h>
#include <SocketApplication.h>
#include <EventManager.h>
#include <MessageQueueManager.h>
#include <UserSessionManager.h>

namespace ArmyAntServer{
class UserSessionManager;
class MessageQueueManager;

class ServerMain{
public:
	ServerMain();
	~ServerMain();

public:
	int32 main();
	bool send();

public:
	UserSessionManager & getUserSessionManager();
	MessageQueueManager & getMessageQueueManager();

private:
	int32 parseConfig();
	int32 modulesInitialization();

private:
	void onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	void onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body);

private:
	bool debug;
	uint16 port;
	MessageQueue* msgQueue;

private:
	SocketApplication socket;
	Logger logger;
	EventManager eventMgr;
	MessageQueueManager msgQueueMgr;
	UserSessionManager sessionMgr;

	std::map<int32, std::thread> threads;

	AA_FORBID_COPY_CTOR(ServerMain);
	AA_FORBID_ASSGN_OPR(ServerMain);
};

}

#endif // SERVER_MAIN_H_20180522
