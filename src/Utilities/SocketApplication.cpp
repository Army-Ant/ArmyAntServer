#include <SocketApplication.h>

namespace ArmyAntServer{


static bool onConnected(uint32 index, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);

	if(self->eventCallback == nullptr){
		return false;
	}

	if(self->clients.find(index)!=self->clients.end()){
		self->eventCallback(SocketApplication::EventType::ErrorReport, index, "New client connected into , but the same IP and port connection has found ! Please check the code");
		return false;
	} else{
		self->connectMutex.lock();
		self->clients.insert(std::make_pair(index, ClientInformation(index, ClientStatus::Conversation)));
		self->connectMutex.unlock();
		self->eventCallback(SocketApplication::EventType::Connected, index, "New client connected");
		return true;
	}
}

static void onDisonnected(uint32 index, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	if(self->eventCallback == nullptr){
		return;
	}
	
	auto findedClient = self->clients.find(index);
	if(findedClient == self->clients.end()){
		self->eventCallback(SocketApplication::EventType::ErrorReport, index, "Detected a client disconnected, but cannot find it in list");
	} else{
		self->eventCallback(SocketApplication::EventType::Disconnected, index, "Client disconnected");
	}
	self->clients.erase(findedClient);
}

static void onReceived(uint32 index, uint8*data, mac_uint datalen, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	auto client = self->tcpSocket.getClientByIndex(index);
	self->getLogger().pushLog((ArmyAnt::String("Received client data, ip: ") + client.clientAddr->getStr() + " , port: " + int64(client.clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");
	self->getLogger().pushLog((ArmyAnt::String("Data content: ") + reinterpret_cast<const char*>(data)).c_str(), Logger::AlertLevel::Info, "ServerMain");
}

static bool onSendResponse(mac_uint sendedSize, uint32 retriedTimes, int32 index, void*sendedData, uint64 len, void* pThis){
	return false;
}

static void onErrorReport(ArmyAnt::SocketException err, const ArmyAnt::IPAddr&addr, uint16 port, ArmyAnt::String functionName, void*pThis){
	auto self = static_cast<SocketApplication*>(pThis);
	uint32 index = self->tcpSocket.getIndexByAddrPort(addr, port);
	if(self->eventCallback != nullptr){
		self->eventCallback(SocketApplication::EventType::ErrorReport, 
							index, 
							ArmyAnt::String("Found error, code:") + int64(err.code) + ", message: " + err.message);
	}
}

/***********************************************************/

ClientInformation::ClientInformation(const int64&index, ClientStatus status)
:index(index), status(status){
}

ClientInformation::~ClientInformation(){
}


SocketApplication::SocketApplication(Logger&logger)
	:logger(logger), tcpSocket(AA_INT32_MAX), eventCallback(nullptr), receiveCallback(nullptr), clients(), connectMutex(){
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

ArmyAnt::TCPServer&SocketApplication::getSocket(){
	return tcpSocket;
}

const std::map<int64, ClientInformation>&SocketApplication::getClientList()const{
	return clients;
}

bool SocketApplication::start(uint16 port, bool isIpv6){
	if(tcpSocket.isStarting())
		return false;
	if(receiveCallback == nullptr || eventCallback == nullptr)
		return false;

	tcpSocket.setConnectCallBack(onConnected, this);
	tcpSocket.setDisconnectCallBack(onDisonnected, this);
	tcpSocket.setGettingCallBack(onReceived, this);
	tcpSocket.setSendingResponseCallBack(onSendResponse, this);
	tcpSocket.setErrorReportCallBack(onErrorReport, this);

	return tcpSocket.start(port, isIpv6);
}

bool SocketApplication::stop(){
	if(!tcpSocket.isStarting())
		return true;
	return tcpSocket.stop(20000);
}

uint64 SocketApplication::send(uint64 clientId, int32 serials, MessageType type, int32 extendVersion, void*extend, void*content){
	if(!tcpSocket.isStarting())
		return false;

	int64 index = 0;
	auto client = clients.find(clientId);
	if(client == clients.end())
		return false;

	int32 extendLength = 0;
	switch(extendVersion){
		case 1:
		{
			System::SocketExtendNormal_V0_0_0_1*head = static_cast<System::SocketExtendNormal_V0_0_0_1*>(extend);
			extendLength = head->ByteSize();
			break;
		}
		default:
			return false;
	}

	MessageBaseHead head{
		serials,
		type,
		extendVersion,
		extendLength,
	};


	//tcpSocket.send()

	return 0;
}

} // namespace ArmyAntServer