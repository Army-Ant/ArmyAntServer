#ifndef SOCKET_CLIENT_APPLICATION_H_20180825
#define SOCKET_CLIENT_APPLICATION_H_20180825

#include "SocketStructs.h"

namespace ArmyAntServer{

class SocketClientApplication{
public:
	enum class EventType : int8{
		Unknown,
		Connected,
		LostServer,
		SendingResponse,
		ErrorReport,
	};

	typedef std::function<void(EventType type, ArmyAnt::String content)> EventCallback;
	typedef std::function<void(const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode,
							   void*body)> 		ReceiveCallback;

public:
	SocketClientApplication();
	~SocketClientApplication();

public:
	bool setEventCallback(EventCallback cb);
	bool setReceiveCallback(ReceiveCallback cb);
	void setWillReconnectWhenLost(bool reconnect);
	ArmyAnt::TCPClient&getSocket();

	bool connect(ArmyAnt::IPAddr&ip, uint16 port, uint16 localPort, bool isAsync, uint32 maxBufferLength);
	bool disconnect();
	int32 send(int32 serials, MessageType type, int32 extendVersion, google::protobuf::Message&extend, void*content);

private:
	ArmyAnt::TCPClient tcpSocket;
	EventCallback eventCallback;
	ReceiveCallback receiveCallback;

	std::mutex rwMutex;
	uint32 bufferLength;

	int32 counter;
	std::map<int32, uint8*>waitingResponseSended;
	uint8*receivingBuffer;
	uint32 receivingBufferEnd;
	bool reconnectLost;

	friend static bool onClientConnected(bool isSucceed, void*pThis);
	friend static bool onClientLostServer(void*pThis);
	friend static void onClientReceived(uint8*data, mac_uint datalen, void*pThis);
	friend static bool onClientSendResponse(mac_uint sendedSize, uint32 retriedTimes, int32, void*sendedData, uint64 len, void* pThis);
	friend static void onClientErrorReport(ArmyAnt::SocketException err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis);

};

}


#endif // SOCKET_CLIENT_APPLICATION_H_20180825
