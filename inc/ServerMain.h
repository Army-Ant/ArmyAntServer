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
	// Initialize Server
	// 初始化服务器
	ServerMain();
	~ServerMain();

public:
	// Run the server, start parameter is set in setting file : Constants::SERVER_CONFIG_FILE_PATH
	// 启动服务器, 根据日志里的配置
	int32 main();
	bool send();

public:
	// Get the UserSessionManager to set user session logic
	// 获取用户会话管理器, 以便服务器逻辑部分能够设置和处理用户会话
	UserSessionManager & getUserSessionManager();
	// Get the UserSessionManager to send message to serverMain ( or to other parts )
	// 获取消息队列管理器, 以便新建消息队列, 或者向服务器其他模块发送消息
	MessageQueueManager & getMessageQueueManager();

private:
	// 解析配置文件的函数
	int32 parseConfig();
	// 初始化各模块的函数
	int32 modulesInitialization();

private:
	// 收到 Socket 本地事件的处理函数
	void onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	// 收到 Socket 网络消息的处理函数
	void onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body);

private:
	bool debug;    // 是否处于调试模式, 由配置文件决定此参数
	uint16 port;    // ServerMain服务器端口号, 由配置文件决定此参数
	MessageQueue* msgQueue;   // ServerMain消息队列

private:
	SocketApplication socket;   // Socket会话类对象
	Logger logger;    // ServerMain日志文件
	EventManager eventMgr;    // 事件管理器
	MessageQueueManager msgQueueMgr;    // 消息队列管理器
	UserSessionManager sessionMgr;    // 用户会话管理器

	std::map<int32, std::thread> threads;

	AA_FORBID_COPY_CTOR(ServerMain);
	AA_FORBID_ASSGN_OPR(ServerMain);
};

}

#endif // SERVER_MAIN_H_20180522
