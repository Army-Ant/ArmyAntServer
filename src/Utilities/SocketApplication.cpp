#include <SocketApplication.h>

namespace ArmyAntServer{

bool onConnected(const ArmyAnt::IPAddr&clientAddr, uint16 clientPort, void*pThis){
	auto pSelf = static_cast<SocketApplication*>(pThis);
	pSelf->getLogger().pushLog((ArmyAnt::String("Client connected, ip: ") + clientAddr.getStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");
	return true;
}

void onDisonnected(const ArmyAnt::IPAddr& clientAddr, uint16 clientPort, void*pThis){
	auto pSelf = static_cast<SocketApplication*>(pThis);
	pSelf->getLogger().pushLog((ArmyAnt::String("Client disconnected, ip: ") + clientAddr.getStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");

}

void onReceived(const ArmyAnt::IPAddr&addr, uint16 port, uint8*data, mac_uint datalen, void*pThis){
	auto pSelf = static_cast<SocketApplication*>(pThis);
	pSelf->getLogger().pushLog((ArmyAnt::String("Received client data, ip: ") + addr.getStr() + " , port: " + int64(port)).c_str(), Logger::AlertLevel::Info, "ServerMain");
	pSelf->getLogger().pushLog((ArmyAnt::String("Data content: ") + reinterpret_cast<const char*>(data)).c_str(), Logger::AlertLevel::Info, "ServerMain");
}

/***********************************************************/

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
	if(tcpSocket.isStarting())
		return false;
	if(receiveCallback == nullptr || eventCallback == nullptr)
		return false;
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