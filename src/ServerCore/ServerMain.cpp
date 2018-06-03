#include <ArmyAnt.h>
#include <ServerMain.h>

#define AA_HANDLE_MANAGER ArmyAnt::ClassPrivateHandleManager<ServerMain, ServerMain_Private>::getInstance()

namespace ArmyAntServer {

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
		pSelf->logger.pushLog((ArmyAnt::String("Client connected, ip: ") + clientAddr.getStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");
		return true;
	}

	void ServerMain_Private::onDisonnected(const ArmyAnt::IPAddr& clientAddr, uint16 clientPort, void*pThis) {
		auto pSelf = static_cast<ServerMain_Private*>(pThis);
		pSelf->logger.pushLog((ArmyAnt::String("Client disconnected, ip: ") + clientAddr.getStr() + " , port: " + int64(clientPort)).c_str(), Logger::AlertLevel::Info, "ServerMain");

	}

	void ServerMain_Private::onReceived(const ArmyAnt::IPAddr&addr, uint16 port, uint8*data, mac_uint datalen, void*pThis) {
		auto pSelf = static_cast<ServerMain_Private*>(pThis);
		pSelf->logger.pushLog((ArmyAnt::String("Received client data, ip: ") + addr.getStr() + " , port: " + int64(port)).c_str(), Logger::AlertLevel::Info, "ServerMain");
		pSelf->logger.pushLog((ArmyAnt::String("Data content: ") + reinterpret_cast<const char*>(data)).c_str(), Logger::AlertLevel::Info, "ServerMain");
	}

	ServerMain::ServerMain() {
		AA_HANDLE_MANAGER.GetHandle(this);
	}

	ServerMain::~ServerMain() {
		delete AA_HANDLE_MANAGER.ReleaseHandle(this);
	}

	int32 ServerMain::start() {
		int exitcode = 0;
		std::cin >> exitcode;
		while (exitcode != 200) {
			std::cin >> exitcode;
		}
		return 0;
	}

}


#undef AA_HANDLE_MANAGER
