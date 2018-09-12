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

namespace ArmyAntServer{
class UserSessionManager;
class MessageQueueManager;

class ServerMain{
public:
	// Initialize Server
	// ��ʼ��������
	ServerMain();
	~ServerMain();

public:
	// Run the server, start parameter is set in setting file : Constants::SERVER_CONFIG_FILE_PATH
	// ����������, ������־�������
	int32 main();
	bool send();

public:
	// Get the UserSessionManager to set user session logic
	// ��ȡ�û��Ự������, �Ա�������߼������ܹ����úʹ����û��Ự
	UserSessionManager & getUserSessionManager();
	// Get the MessageQueueManager to send message to serverMain ( or to other parts )
	// ��ȡ��Ϣ���й�����, �Ա��½���Ϣ����, ���������������ģ�鷢����Ϣ
	MessageQueueManager & getMessageQueueManager();

private:
	// ���������ļ��ĺ���
	int32 parseConfig();
	// ��ʼ����ģ��ĺ���
	int32 modulesInitialization();
	// �������ݿ����, ��һֱ��������ֱ���ɹ�
	void connectDBProxy(bool isReconnect = false);
	// �˳�ʱ������Դ
	int32 modulesUninitialization();

private:
	// �յ� Socket �����¼��Ĵ�����
	void onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	// �յ� Socket ������Ϣ�Ĵ�����
	void onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, void*body);

	// �յ� DBProxy ������صı����¼��Ĵ�����
	void onDBConnectorEvent(SocketClientApplication::EventType type, ArmyAnt::String content);
	// �յ� DBProxy ������Ϣ�Ĵ�����
	void onDBConnectorReceived(const MessageBaseHead & head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, void * body);

private:
	bool debug;    // �Ƿ��ڵ���ģʽ, �������ļ������˲���
	uint16 port;    // ServerMain�������˿ں�, �������ļ������˲���
	MessageQueue* msgQueue;   // ServerMain��Ϣ����

private:
	SocketApplication socket;   // Socket�Ự�����
	Logger logger;    // ServerMain��־�ļ�
	EventManager eventMgr;    // �¼�������
	MessageQueueManager msgQueueMgr;    // ��Ϣ���й�����
	UserSessionManager sessionMgr;    // �û��Ự������

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
