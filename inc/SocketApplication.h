#ifndef SOCKET_APPLICATION_H_20180604
#define SOCKET_APPLICATION_H_20180604

#include <map>

#include "SocketStructs.h"

namespace ArmyAntServer{

struct ClientInformation{
	uint32 index;
	ClientStatus status;

	ClientInformation(const int32 index, ClientStatus status);
	ClientInformation(ClientInformation&&moved);
	~ClientInformation();

private:
	int32 counter;
	std::map<int32, uint8*>waitingResponseSended;
	uint8*receivingBuffer;
	uint32 receivingBufferEnd;

	std::mutex rwMutex;

	void setMaxBufferLength(uint32 bufferLength);

	friend class SocketApplication;

	AA_FORBID_ASSGN_OPR(ClientInformation);
	AA_FORBID_COPY_CTOR(ClientInformation);
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

	typedef std::function<void(EventType type, const uint32 clientIndex, ArmyAnt::String content)> EventCallback;
	typedef std::function<void(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode,
							   void*body)> 		ReceiveCallback;

public:
	SocketApplication();
	~SocketApplication();

public:
	bool setEventCallback(EventCallback cb);
	bool setReceiveCallback(ReceiveCallback cb);
	ArmyAnt::TCPServer&getSocket();
	const std::map<uint32, ClientInformation*>&getClientList()const;

	bool start(uint16 port, uint32 maxBufferLength, bool isIpv6 = false);
	bool stop();
	int32 send(uint32 clientId, int32 serials, MessageType type, int32 extendVersion, google::protobuf::Message&extend, void*content);

private:
	ArmyAnt::TCPServer tcpSocket;
	EventCallback eventCallback;
	ReceiveCallback receiveCallback;
	std::map<uint32, ClientInformation*> clients;

	std::mutex connectMutex;
	uint32 bufferLength;

	friend class ClientInformation;

	static bool onServerConnected(uint32 index, void*pThis);
	static void onServerDisonnected(uint32 index, void*pThis);
	static void onServerReceived(uint32 index, uint8*data, mac_uint datalen, void*pThis);
	static bool onServerSendResponse(mac_uint sendedSize, uint32 retriedTimes, uint32 index, void*sendedData, uint64 len, void* pThis);
	static void onServerErrorReport(ArmyAnt::SocketException err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis);

private:
	AA_FORBID_ASSGN_OPR(SocketApplication);
	AA_FORBID_COPY_CTOR(SocketApplication);
};

} // namespace ArmyAntServer


#endif // SOCKET_APPLICATION_H_20180604
