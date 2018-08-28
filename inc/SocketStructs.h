#ifndef SOCKET_STRUCTS_H_20180825
#define SOCKET_STRUCTS_H_20180825

#include <functional>
#include <mutex>

#include <AASocket.h>
#include <ArmyAntMessage/System/SocketHead.pb.h>

namespace ArmyAntServer{

enum class MessageType : int32{
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


struct MessageBaseHead{
	int32 serials;
	MessageType type;
	int32 extendVersion;
	int32 extendLength;
};


}

#endif // SOCKET_STRUCTS_H_20180825
