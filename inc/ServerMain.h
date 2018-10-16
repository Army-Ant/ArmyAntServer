#ifndef SERVER_MAIN_H_20180522
#define SERVER_MAIN_H_20180522

#include <map>

#include <AADefine.h>

#include <Logger.h>
#include <SocketApplication.h>
#include <SocketClientApplication.h>
#include <EventManager.h>
#include <MessageQueueManager.h>
#include <UserSessionManager.h>
#include <SubApplicationManager.h>

namespace ArmyAntServer{

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
	bool send(bool isWebSocket, uint32 clientId, int32 serials, MessageType type, int32 extendVersion, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*content);

public:
	Logger& getLogger();
	// Get the EventManager to register or listen events
	// 获取事件管理器, 以便注册和监听本地事件或网络消息
	EventManager & getEventManager();
	// Get the UserSessionManager to set user session logic
	// 获取用户会话管理器, 以便服务器逻辑部分能够设置和处理用户会话
	UserSessionManager & getUserSessionManager();
	// Get the MessageQueueManager to send message to serverMain ( or to other parts )
	// 获取消息队列管理器, 以便新建消息队列, 或者向服务器其他模块发送消息
	MessageQueueManager & getMessageQueueManager();
	// Get the SubApplicationManager to do something between subapplications
	// 获取子应用管理器
	SubApplicationManager & getSubApplicationManager();

private:
	// 解析配置文件的函数
	int32 parseConfig();
	// 初始化各模块的函数
	int32 modulesInitialization();
	// 连接数据库代理, 会一直尝试连接直到成功
	void connectDBProxy(bool isReconnect = false);
	// 退出时销毁资源
	int32 modulesUninitialization();

private:
	// 收到 Socket 连接的消息报告的处理函数
	void onSocketEvent(bool isWebsocket, SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	// 收到 Socket 网络消息的处理函数
	void onSocketReceived(bool isWebsocket, const uint32 clientId, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*body);

	// 收到 DBProxy 连接的消息报告的处理函数
	void onDBConnectorEvent(SocketClientApplication::EventType type, ArmyAnt::String content);
	// 收到 DBProxy 网络消息的处理函数
	void onDBConnectorReceived(const MessageBaseHead & head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void * body);

private:
	bool debug;    // 是否处于调试模式, 由配置文件决定此参数
	uint16 normalSocketPort;    // ServerMain服务器端口号, 由配置文件决定此参数
	uint16 webSocketPort;    // ServerMain服务器websocket端口号, 由配置文件决定此参数
	MessageQueue* msgQueue;   // ServerMain消息队列

private:
	Logger logger;    // ServerMain日志文件
	MessageQueueManager msgQueueMgr;    // 消息队列管理器
	SocketApplication normalSocket;   // Socket会话类对象
	SocketApplication webSocket;   // websocket会话类对象
	UserSessionManager sessionMgr;    // 用户会话管理器
	EventManager eventMgr;    // 事件管理器
	SubApplicationManager appMgr;

private:
	SocketClientApplication dbConnector;
	ArmyAnt::IPAddr* dbAddr;
	uint16 dbPort;
	uint16 dbLocalPort;

	AA_FORBID_COPY_CTOR(ServerMain);
	AA_FORBID_ASSGN_OPR(ServerMain);
};

}

#endif // SERVER_MAIN_H_20180522
