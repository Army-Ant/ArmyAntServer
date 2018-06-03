#include <SocketApplication.h>

namespace ArmyAntServer{

SocketApplication::SocketApplication(Logger&logger)
	:logger(logger), tcpSocket(AA_INT32_MAX){
}

SocketApplication::~SocketApplication(){

}

bool SocketApplication::setEventCallback(SocketApplication::EventCallback cb){
	eventCallback = cb;
	return true;
}

bool SocketApplication::setReceiveCallback(SocketApplication::ReceiveCallback cb){
	receiveCallback = cb;
	return true;
}

Logger& SocketApplication::getLogger(){
	return logger;
}

bool SocketApplication::start(uint16 port, bool isIpv6){
	// TODO

	return false;
}

bool SocketApplication::stop(){
	// TODO

	return false;
}

uint64 SocketApplication::send(uint64 clientId, int32 serials, MessageType type, int32 extendVersion, void*extend, void*content){
	// TODO

	return 0;
}

} // namespace ArmyAntServer