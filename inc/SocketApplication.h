#ifndef SOCKET_APPLICATION_H_20180604
#define SOCKET_APPLICATION_H_20180604

#include <functional>

#include <AASocket.h>
#include "Logger.h"

namespace ArmyAntServer{

enum class MessageType : int8{
	Unknown,
	Normal,
	File,
};

#pragma pack(0)
struct MessageBaseHead{
	int32 serials;
	MessageType type;
	int32 extendVersion;
	int32 extendLength;
};
#pragma pack()

class SocketApplication{
public:
	enum class EventType : int8{
		Unknown,
		Started,
		Stopping,
		Connected,
		Disconnected,
		SendingResponse,
		ErrorReport,
	};

	typedef std::function<void()> EventCallback;
	typedef std::function<void()> ReceiveCallback;

public:
	SocketApplication(Logger&log);
	~SocketApplication();

public:
	bool setEventCallback(EventCallback cb);
	bool setReceiveCallback(ReceiveCallback cb);
	Logger&getLogger();

	bool start(uint16 port, bool isIpv6 = false);
	bool stop();
	uint64 send(uint64 clientId, int32 serials, MessageType type, int32 extendVersion, void*extend, void*content);

public:
	AA_FORBID_ASSGN_OPR(SocketApplication);
	AA_FORBID_COPY_CTOR(SocketApplication);

private:
	Logger & logger;
	EventCallback eventCallback;
	ReceiveCallback receiveCallback;

	ArmyAnt::TCPServer tcpSocket;
};

} // namespace ArmyAntServer


#endif // SOCKET_APPLICATION_H_20180604
