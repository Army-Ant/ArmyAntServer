#ifndef DBPROXY_MAIN_H_20180827
#define DBPROXY_MAIN_H_20180827

#include <map>

#include <AADefine.h>

#include <Logger.h>
#include <SocketApplication.h>
#include <EventManager.h>
#include <MessageQueueManager.h>

#include <mysqlBridge.h>

namespace ArmyAntDBProxy{

class DBProxyMain{
public:
	// Initialize DBProxy
	// 初始化
	DBProxyMain();
	~DBProxyMain();

public:
	// Run the server, start parameter is set in setting file : Constants::DB_PROXY_CONFIG_FILE_PATH
	// 启动服务器, 根据日志里的配置
	int32 main();
	bool send();

public:
	// Get the MessageQueueManager to send message to DBProxyMain ( or to other parts )
	// 获取消息队列管理器, 以便新建消息队列, 或者向服务器其他模块发送消息
	ArmyAntServer::MessageQueueManager & getMessageQueueManager();

private:
	// 解析配置文件的函数
	int32 parseConfig();
	// 初始化各模块的函数
	int32 modulesInitialization();
	// 退出时销毁资源
	int32 modulesUninitialization();

private:
	// 收到 Socket 本地事件的处理函数
	void onSocketEvent(ArmyAntServer::SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	// 收到 Socket 网络消息的处理函数
	void onSocketReceived(const uint32 clientIndex, const ArmyAntServer::MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body);

private:
	bool debug;    // 是否处于调试模式, 由配置文件决定此参数
	uint16 port;    // DBProxyMain服务器端口号, 由配置文件决定此参数
	ArmyAntServer::MessageQueue* msgQueue;   // DBProxyMain消息队列

private:
	ArmyAntServer::SocketApplication socket;   // Socket会话类对象
	ArmyAntServer::Logger logger;    // DBProxyMain日志文件
	ArmyAntServer::EventManager eventMgr;    // 事件管理器
	ArmyAntServer::MessageQueueManager msgQueueMgr;    // 消息队列管理器

private:
	MySqlBridge mysqlBridge;
	ArmyAnt::String mysqlServerHost;
	ArmyAnt::String mysqlUsername;
	ArmyAnt::String mysqlPassword;

	AA_FORBID_COPY_CTOR(DBProxyMain);
	AA_FORBID_ASSGN_OPR(DBProxyMain);
};

}

#endif // DBPROXY_MAIN_H_20180827
