#include <ArmyAnt.h>
#include <ServerMain.h>

#include <functional>
#include <thread>

#include <ServerConstants.h>
#include <ArmyAntMessage/System/SocketHead.pb.h>

#include <EchoApp.h>
#include <HuolongServer.h>

static const char* const LOGGER_TAG = "ServerMain";

namespace ArmyAntServer{


ServerMain::ServerMain()
	:debug(false), normalSocketPort(0), webSocketPort(0), msgQueue(nullptr), normalSocket(false), webSocket(true), logger(), msgQueueMgr(), sessionMgr(msgQueueMgr, logger), eventMgr(sessionMgr, logger), appMgr(logger), dbConnector(false), dbAddr(nullptr), dbPort(0)
{
	sessionMgr.setEventManager(eventMgr);
}

ServerMain::~ServerMain(){}

int32 ServerMain::main(){
	// 1. 读取配置
	auto parseRes = parseConfig();
	if(parseRes != Constants::ServerMainReturnValues::normalExit){
		return parseRes;
	}

	// 2. 初始化各个模块, 注意顺序
	auto initRes = modulesInitialization();
	if(initRes != Constants::ServerMainReturnValues::normalExit){
		return initRes;
	}

	// 3. 初始化并开启 TCP server 的 socket 以及 websocket
	normalSocket.setEventCallback(std::bind(&ServerMain::onSocketEvent, this, false, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	normalSocket.setReceiveCallback(std::bind(&ServerMain::onSocketReceived, this, false, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8, std::placeholders::_9));
	auto socketStartRes = normalSocket.start(normalSocketPort, 16384, false);  // 开启 socket
	if(!socketStartRes){
		logger.pushLog("Server socket started failed", ArmyAnt::Logger::AlertLevel::Fatal, LOGGER_TAG);
		return Constants::ServerMainReturnValues::serverStartFailed;
	}
	webSocket.setEventCallback(std::bind(&ServerMain::onSocketEvent, this, true, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	webSocket.setReceiveCallback(std::bind(&ServerMain::onSocketReceived, this, true, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8, std::placeholders::_9));
	socketStartRes = webSocket.start(webSocketPort, 16384, false);  // 开启 websocket
	if(!socketStartRes){
		logger.pushLog("Server websocket started failed", ArmyAnt::Logger::AlertLevel::Fatal, LOGGER_TAG);
		return Constants::ServerMainReturnValues::serverStartFailed;
	}
	if(!appMgr.startAllApplications()){  // 开启所有 subApp
		logger.pushLog("An sub-application started failed", ArmyAnt::Logger::AlertLevel::Fatal, LOGGER_TAG);
		return Constants::ServerMainReturnValues::serverStartFailed;
	}
	logger.pushLog("Server started", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);

	// 4. 开始监听主线程消息队列
	msgQueue = msgQueueMgr.createQueue(SpecialUserIndex::SERVER_MAIN_THREAD);
	int32 retVal = Constants::ServerMainReturnValues::normalExit;
	bool exitCommand = false;
	while(!exitCommand){
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		if(msgQueue->hasMessage()){
			auto msg = msgQueue->popMessage();
			switch(msg.id){
				case Constants::ServerMainMsg::exitMainThread:
					webSocket.stop();
					normalSocket.stop();
					retVal = msg.data;
					logger.pushLog("Server stopped", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
					exitCommand = true;
					break;
				case Constants::ServerMainMsg::dbProxyNeedReconnect:
					connectDBProxy(true);
					break;
				default:
					logger.pushLog("ServerMain received an unknown message, code:" + ArmyAnt::String(msg.id) + ", data:" + int64(msg.data), ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
			}
		}
	}

	// 5. 退出时销毁资源
	auto uninitRes = modulesUninitialization();
	logger.pushLog("Program over", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	return retVal;
}

bool ServerMain::send(bool isWebSocket, uint32 clientId, int32 serials, MessageType type, int32 extendVersion, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*content){
	switch(extendVersion){
		case 1:
		{
			ArmyAntMessage::System::SocketExtendNormal_V0_0_0_1 extend;
			extend.set_app_id(appid);
			extend.set_content_length(contentLength);
			extend.set_message_code(messageCode);
			extend.set_conversation_code(conversationCode);
			extend.set_conversation_step_index(conversationStepIndex);
			extend.set_conversation_step_type(conversationStepType);
			if(isWebSocket)
				webSocket.send(clientId / 2, serials, type, extendVersion, extend, content);
			else
				normalSocket.send(clientId / 2, serials, type, extendVersion, extend, content);
		}
		default:
			logger.pushLog(ArmyAnt::String("Sending a network message with an unknown head version: ") + int64(extendVersion), ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
			return false;
	}
	return true;
}

ArmyAnt::Logger & ServerMain::getLogger(){
	return logger;
}

EventManager & ServerMain::getEventManager(){
	return eventMgr;
}

UserSessionManager&ServerMain::getUserSessionManager(){
	return sessionMgr;
}

MessageQueueManager&ServerMain::getMessageQueueManager(){
	return msgQueueMgr;
}

SubApplicationManager & ServerMain::getSubApplicationManager(){
	return appMgr;
}

int32 ServerMain::parseConfig(){
	ArmyAnt::File configJson;
	// 打开设置文件
	bool openRes = configJson.Open(Constants::SERVER_CONFIG_FILE_PATH);
	if(!openRes){
		return Constants::ServerMainReturnValues::openConfigFileFailed;
	}
	// 读取设置内容
	auto jsonFileLen = configJson.GetLength();
	char* buf = new char[jsonFileLen + 20];
	memset(buf, 0, jsonFileLen + 20);
	bool readRes = configJson.Read(buf);
	configJson.Close();
	if(!readRes){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		return Constants::ServerMainReturnValues::parseConfigJsonFailed;
	}
	// 序列化设置项
	auto json = ArmyAnt::JsonUnit::create(buf);
	auto pJo = dynamic_cast<ArmyAnt::JsonObject*>(json);
	if(pJo == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}

	// 保存设置项到内存
	auto&jo = *pJo;
	auto jdebug = jo.getChild("debug");
	auto pjdebug = dynamic_cast<ArmyAnt::JsonBoolean*>(jdebug);
	if(pjdebug == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	debug = pjdebug->getBoolean();

	auto jnormalSocketPort = jo.getChild("normalSocketPort");
	auto pjnormalSocketPort = dynamic_cast<ArmyAnt::JsonNumeric*>(jnormalSocketPort);
	if(pjnormalSocketPort == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	normalSocketPort = pjnormalSocketPort->getInteger();

	auto jwebsocketPort = jo.getChild("websocketPort");
	auto pjwebsocketPort = dynamic_cast<ArmyAnt::JsonNumeric*>(jwebsocketPort);
	if(pjwebsocketPort == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	webSocketPort = pjwebsocketPort->getInteger();

	auto jlogPath = jo.getChild("logPath");
	auto logFilePath = dynamic_cast<ArmyAnt::JsonString*>(jlogPath);
	if(logFilePath == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	logger.setLogFile(logFilePath->getString());

	// 文件日志筛选级别
	auto logFileLevel = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("logFileLevel"));
	if(logFileLevel->getString() == ArmyAnt::String("verbose")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Verbose);
	} else	if(logFileLevel->getString() == ArmyAnt::String("debug")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Debug);
	} else	if(logFileLevel->getString() == ArmyAnt::String("info")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Info);
	} else	if(logFileLevel->getString() == ArmyAnt::String("import")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Import);
	} else	if(logFileLevel->getString() == ArmyAnt::String("warning")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Warning);
	} else	if(logFileLevel->getString() == ArmyAnt::String("error")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Error);
	} else	if(logFileLevel->getString() == ArmyAnt::String("fatal")){
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Fatal);
	} else{
		logger.setFileLevel(ArmyAnt::Logger::AlertLevel::Verbose);
	}

	// 控制台日志筛选级别
	auto logConsoleLevel = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("logConsoleLevel"));
	if(logConsoleLevel->getString() == ArmyAnt::String("verbose")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Verbose);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("debug")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Debug);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("info")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Info);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("import")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Import);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("warning")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Warning);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("error")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Error);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("fatal")){
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Fatal);
	} else{
		logger.setConsoleLevel(ArmyAnt::Logger::AlertLevel::Import);
	}

	// DBProxy 地址
	auto jDBProxyAddr = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("dbProxyAddr"));
	if(jDBProxyAddr == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	dbAddr = ArmyAnt::IPAddr::create(jDBProxyAddr->getString());
	auto jDBProxyPort = dynamic_cast<ArmyAnt::JsonNumeric*>(jo.getChild("dbProxyPort"));
	if(jDBProxyPort == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	dbPort = uint16(jDBProxyPort->getLong());

	ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
	ArmyAnt::JsonUnit::release(json);
	logger.pushLog("Config loading successful", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

int32 ServerMain::modulesInitialization(){
	// 连接数据库代理进程
	connectDBProxy();

	// 初始化所有子程序
	appMgr.registerApplication(Constants::ServerMainAppid::simpleEchoApp, new EchoApp(Constants::ServerMainAppid::simpleEchoApp, *this));
	appMgr.registerApplication(Constants::ServerMainAppid::huolongServer, new HuolongServer(Constants::ServerMainAppid::huolongServer, *this));


	logger.pushLog("All modules initialized successful", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

void ServerMain::connectDBProxy(bool isReconnect){
	dbConnector.setEventCallback(std::bind(&ServerMain::onDBConnectorEvent, this, std::placeholders::_1, std::placeholders::_2));
	dbConnector.setReceiveCallback(std::bind(&ServerMain::onDBConnectorReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8));
	auto ret = dbConnector.connect(*dbAddr, dbPort, false, 8192);
	while(!ret){
		logger.pushLog("DBproxy connected failed", ArmyAnt::Logger::AlertLevel::Error, LOGGER_TAG);
		ret = dbConnector.connect(*dbAddr, dbPort, false, 8192);
	}
	if(isReconnect)
		logger.pushLog("DBproxy reconnected", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	else
		logger.pushLog("DBproxy connected", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
}

int32 ServerMain::modulesUninitialization(){
	// 断开数据库代理的连接
	dbConnector.disconnect();
	ArmyAnt::Fragment::AA_SAFE_DEL(dbAddr);

	// 关闭子应用
	appMgr.stopAllApplications();
	delete appMgr.unregisterApplication(Constants::ServerMainAppid::simpleEchoApp);

	logger.pushLog("All modules uninitialized OK", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

void ServerMain::onSocketEvent(bool isWebsocket, SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content){
	uint32 clientId = isWebsocket ? clientIndex * 2 + 1 : clientIndex * 2;
	switch(type){
		case SocketApplication::EventType::Connected:
			logger.pushLog("New client connected! client index: " + ArmyAnt::String(int64(clientIndex)) + ", client id: "+ ArmyAnt::String(int64(clientId)), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
			if(isWebsocket)
				sessionMgr.createUserSession(clientId, webSocket, clientIndex);
			else
				sessionMgr.createUserSession(clientId, normalSocket, clientIndex);
			break;
		case SocketApplication::EventType::Disconnected:
			logger.pushLog("Client disconnected! client id: " + ArmyAnt::String(int64(clientId)), ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
			sessionMgr.removeUserSession(clientId);
			break;
		case SocketApplication::EventType::SendingResponse:
			logger.pushLog("Sending to client responsed, client id: " + ArmyAnt::String(int64(clientId)), ArmyAnt::Logger::AlertLevel::Verbose, LOGGER_TAG);
			break;
		case SocketApplication::EventType::ErrorReport:
			logger.pushLog("Get socket error-report, client id: " + ArmyAnt::String(int64(clientId)) + ", content: " + content, ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
			break;
		case SocketApplication::EventType::Unknown:
			logger.pushLog("Get an unknown socket event, client id: " + ArmyAnt::String(int64(clientId)) + ", content: " + content, ArmyAnt::Logger::AlertLevel::Import, LOGGER_TAG);
			break;
		default:
			logger.pushLog("Get an unknown number of socket event, eventType number: " + ArmyAnt::String(int64(int8(type))) + ", client index: " + int64(clientId) + ", content: " + content, ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
	}
}

void ServerMain::onSocketReceived(bool isWebsocket, const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*body){
	uint32 clientId = isWebsocket ? clientIndex * 2 + 1 : clientIndex * 2;
	logger.pushLog("Received from client id: " + ArmyAnt::String(int64(clientId)) + ", appid: " + int64(appid), ArmyAnt::Logger::AlertLevel::Verbose, LOGGER_TAG);
	eventMgr.dispatchNetworkResponse(head.extendVersion, messageCode, clientId, conversationCode, conversationStepIndex, conversationStepType, body, contentLength);
}

void ServerMain::onDBConnectorEvent(SocketClientApplication::EventType type, ArmyAnt::String content){
	switch(type){
		case SocketClientApplication::EventType::Connected:
			logger.pushLog("DBProxy connected!", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::LostServer:
			logger.pushLog("DBProxy server lost!", ArmyAnt::Logger::AlertLevel::Info, LOGGER_TAG);
			msgQueue->pushMessage(Message{Constants::ServerMainMsg::dbProxyNeedReconnect, 0, nullptr});
			break;
		case SocketClientApplication::EventType::SendingResponse:
			logger.pushLog("Sending to DBProxy responsed", ArmyAnt::Logger::AlertLevel::Verbose, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::ErrorReport:
			logger.pushLog("Get DBProxy socket error-report, content: " + content, ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::Unknown:
			logger.pushLog("Get an unknown socket event from DBProxy socket, content: " + content, ArmyAnt::Logger::AlertLevel::Import, LOGGER_TAG);
			break;
		default:
			logger.pushLog("Get an unknown nomber of socket event, eventType number: " + ArmyAnt::String(int64(int8(type))) + ", content: " + content, ArmyAnt::Logger::AlertLevel::Warning, LOGGER_TAG);
	}
}

void ServerMain::onDBConnectorReceived(const MessageBaseHead & head, uint64 appid, int32 contentLength, int32 messageCode, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void * body){
	logger.pushLog(ArmyAnt::String("Received from DBProxy, appid: ") + int64(appid), ArmyAnt::Logger::AlertLevel::Verbose, LOGGER_TAG);
}


}
