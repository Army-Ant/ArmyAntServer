#ifndef SOCKET_APPLICATION_H_20180604
#define SOCKET_APPLICATION_H_20180604

#include <functional>
#include <map>
#include <mutex>

#include <AASocket.h>
#include "Logger.h"

#include <ArmyAntServer.System.SocketHead.pb.h>

namespace ArmyAntServer{

enum class MessageType : int8{
	Unknown,
	Normal,
	File,
};

enum class ClientStatus : int8{
	Unknown,
	Conversation,
	Busy,
	Disconnected,
	Waiting,
};

#pragma pack(1)
struct MessageBaseHead{
	int32 serials;
	MessageType type;
	int32 extendVersion;
	int32 extendLength;
};
#pragma pack()

struct ClientInformation{
	int64 index;
	ClientStatus status;

	ClientInformation(const int64&index, ClientStatus status);
	~ClientInformation();

private:
	std::map<int64, void*>waitingResponseSended;
	uint8*receivingBuffer;

	friend class SocketApplication;
};

class SocketApplication{
public:
	enum class EventType : int8{
		Unknown,
		Connected,
		Disconnected,
		SendingResponse,
		ErrorReport,
	};

	typedef std::function<void(EventType type, const int64&clientIndex, ArmyAnt::String content)> EventCallback;
	typedef std::function<void(const int64&clientIndex, 
							   const MessageBaseHead&head, 
							   System::SocketExtendNormal_V0_0_0_1&extend, 
							   google::protobuf::Message&body)> 
		ReceiveCallback;

public:
	SocketApplication(Logger&log);
	~SocketApplication();

public:
	bool setEventCallback(EventCallback cb);
	bool setReceiveCallback(ReceiveCallback cb);
	Logger&getLogger();
	ArmyAnt::TCPServer&getSocket();
	const std::map<int64, ClientInformation>&getClientList()const;

	bool start(uint16 port, bool isIpv6 = false);
	bool stop();
	uint64 send(uint64 clientId, int32 serials, MessageType type, int32 extendVersion, void*extend, void*content);

public:
	AA_FORBID_ASSGN_OPR(SocketApplication);
	AA_FORBID_COPY_CTOR(SocketApplication);

private:
	ArmyAnt::TCPServer tcpSocket;
	Logger & logger;
	EventCallback eventCallback;
	ReceiveCallback receiveCallback;
	std::map<int64, ClientInformation> clients;

	std::mutex connectMutex;

	friend static bool onConnected(uint32 index, void*pThis);
	friend static void onDisonnected(uint32 index, void*pThis);
	friend static void onReceived(uint32 index, uint8*data, mac_uint datalen, void*pThis);
	friend static bool onSendResponse(mac_uint sendedSize, uint32 retriedTimes, int32 index, void*sendedData, uint64 len, void* pThis);
	friend static void onErrorReport(ArmyAnt::SocketException err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis);
};

} // namespace ArmyAntServer


#endif // SOCKET_APPLICATION_H_20180604
