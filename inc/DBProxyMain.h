#ifndef DBPROXY_MAIN_H_20180827
#define DBPROXY_MAIN_H_20180827

#include <map>

#include <AADefine.h>

#include <Logger.h>
#include <SocketApplication.h>
#include <EventManager.h>
#include <MessageQueueManager.h>

#include <mysqlBridge.h>

namespace ArmyAntServer{

class DBProxyMain{
public:
	// Initialize DBProxy
	// ��ʼ��
	DBProxyMain();
	~DBProxyMain();

public:
	// Run the DBProxy, start parameter is set in setting file : Constants::DB_PROXY_CONFIG_FILE_PATH
	// ����������, ������־�������
	int32 main();
	bool send();

public:
	// Get the MessageQueueManager to send message to DBProxyMain ( or to other parts )
	// ��ȡ��Ϣ���й�����, �Ա��½���Ϣ����, ���������������ģ�鷢����Ϣ
	MessageQueueManager & getMessageQueueManager();

private:
	// ���������ļ��ĺ���
	int32 parseConfig();
	// ��ʼ����ģ��ĺ���
	int32 modulesInitialization();
	// �˳�ʱ������Դ
	int32 modulesUninitialization();

private:
	// �յ� Socket �����¼��Ĵ�����
	void onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content);
	// �յ� Socket ������Ϣ�Ĵ�����
	void onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, void*body);

private:
	bool debug;    // �Ƿ��ڵ���ģʽ, �������ļ������˲���
	uint16 port;    // DBProxyMain�������˿ں�, �������ļ������˲���
	MessageQueue* msgQueue;   // DBProxyMain��Ϣ����

private:
	SocketApplication socket;   // Socket�Ự�����
	Logger logger;    // DBProxyMain��־�ļ�
	EventManager eventMgr;    // �¼�������
	MessageQueueManager msgQueueMgr;    // ��Ϣ���й�����

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
