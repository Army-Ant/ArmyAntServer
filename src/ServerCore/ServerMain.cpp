#include <ArmyAnt.h>
#include <ServerMain.h>

namespace ArmyAntServer {

	class ServerMain_Private;
	static ArmyAnt::ClassPrivateHandleManager<ServerMain, ServerMain_Private> s_handleManager;

	class ServerMain_Private {
	public:
		ServerMain_Private();
		static bool onConnected(const ArmyAnt::IPAddr&clientAddr, uint16 clientPort, void*pThis);
		static void onDisonnected(const ArmyAnt::IPAddr& clientAddr, uint16 clientPort, void*pThis);
		static void onReceived(const ArmyAnt::IPAddr&addr, uint16 port, uint8*data, mac_uint datalen, void*pThis);

		ArmyAnt::TCPServer coreServer;
		Logger logger;
	};

	ServerMain_Private::ServerMain_Private() : coreServer(0){

	}

	bool ServerMain_Private::onConnected(const ArmyAnt::IPAddr&clientAddr, uint16 clientPort, void*pThis) {
		auto pSelf = static_cast<ServerMain_Private*>(pThis);
		pSelf->logger.pushLog((ArmyAnt::String("Client connected, ip: ") + clientAddr.GetStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");
		return true;
	}

	void ServerMain_Private::onDisonnected(const ArmyAnt::IPAddr& clientAddr, uint16 clientPort, void*pThis) {
		auto pSelf = static_cast<ServerMain_Private*>(pThis);
		pSelf->logger.pushLog((ArmyAnt::String("Client disconnected, ip: ") + clientAddr.GetStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");

	}

	void ServerMain_Private::onReceived(const ArmyAnt::IPAddr&addr, uint16 port, uint8*data, mac_uint datalen, void*pThis) {
		auto pSelf = static_cast<ServerMain_Private*>(pThis);
		pSelf->logger.pushLog((ArmyAnt::String("Received client data, ip: ") + addr.GetStr() + " , port: " + int64(port)).c_str(), Logger::AlertLevel::Info, "ServerMain");
		pSelf->logger.pushLog((ArmyAnt::String("Data content: ") + reinterpret_cast<char*>(data)).c_str(), Logger::AlertLevel::Info, "ServerMain");
	}

	ServerMain::ServerMain() {
		s_handleManager.GetHandle(this);
	}

	ServerMain::~ServerMain() {
		s_handleManager.ReleaseHandle(this);
	}

	int32 ServerMain::start() {
		auto data = s_handleManager.GetDataByHandle(this);
		data->coreServer.SetConnectCallBack(data->onConnected, data);
		data->coreServer.SetDisconnectCallBack(data->onDisonnected, data);
		data->coreServer.SetGettingCallBack(data->onReceived, data);

		data->coreServer.StartServer(4744);

		return 0;
	}

}