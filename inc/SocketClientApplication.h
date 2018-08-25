#ifndef SOCKET_CLIENT_APPLICATION_H_20180825
#define SOCKET_CLIENT_APPLICATION_H_20180825

#include "SocketStructs.h"

namespace ArmyAntServer{

class SocketClientApplication{
public:


public:
	SocketClientApplication();
	~SocketClientApplication();

public:
	bool connect(ArmyAnt::IPAddr&ip, uint16 port, uint16 localPort);
	bool disconnect();
	int32 send(int32 serials, MessageType type, int32 extendVersion, google::protobuf::Message&extend, void*content);

private:
	ArmyAnt::TCPClient tcpSocket;

};

}


#endif // SOCKET_CLIENT_APPLICATION_H_20180825
