#include <HuolongServer.h>
#include <functional>

#include <Logger.h>
#include <ServerMain.h>
#include <EventManager.h>
#include <UserSession.h>
#include <ArmyAntMessage/SubApps/Huolong.pb.h>


namespace ArmyAntServer{

#define NETWORK_CALLBACK(func) std::bind(&func, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)

static const char* const EVENT_TAG = "HuolongServer";
static const char* const LOGGER_TAG = "HuolongServer";

HuolongServer::HuolongServer(int64 appid, ServerMain & server) :SubApplication(appid, server), userList(){}

HuolongServer::~HuolongServer(){}

bool HuolongServer::onStart(){
	userList.clear();
	//server.getEventManager().addGlobalListenerForNetworkResponse(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>(), NETWORK_CALLBACK(HuolongServer::onUserLogin));

	server.getLogger().pushLog("Huolong server started !", Logger::AlertLevel::Info, LOGGER_TAG);
	return true;
}

bool HuolongServer::onStop(){
	//server.getEventManager().removeGlobalListenerForLocalEvent(EVENT_TAG, EventManager::getProtobufMessageCode<ArmyAntMessage::SubApps::C2SM_HuolongLoginRequest>());
	return false;
}



#undef NETWORK_CALLBACK

}

