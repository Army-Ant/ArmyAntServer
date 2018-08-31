#include <ArmyAnt.h>
#include <ServerMain.h>

#include <functional>
#include <thread>

#include <ServerCoreConstants.h>

static const char* const LOGGER_TAG = "ServerMain";

namespace ArmyAntServer{


ServerMain::ServerMain()
	:debug(false), port(0), msgQueue(nullptr), socket(), logger(), eventMgr(), msgQueueMgr(), sessionMgr(*this), dbConnector(), dbAddr(nullptr), dbPort(0), dbLocalPort(0)
{}

ServerMain::~ServerMain(){}

int32 ServerMain::main(){
	// 1. ��ȡ����
	auto parseRes = parseConfig();
	if(parseRes != Constants::ServerMainReturnValues::normalExit){
		return parseRes;
	}

	// 2. ��ʼ������ģ��, ע��˳��
	auto initRes = modulesInitialization();
	if(initRes != Constants::ServerMainReturnValues::normalExit){
		return initRes;
	}

	// 3. ��ʼ�������� socket TCP server
	socket.setEventCallback(std::bind(&ServerMain::onSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	socket.setReceiveCallback(std::bind(&ServerMain::onSocketReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	socket.start(port, 16384, false);
	logger.pushLog("Server started", Logger::AlertLevel::Info, LOGGER_TAG);

	// 4. ��ʼ�������߳���Ϣ����
	msgQueue = msgQueueMgr.createQueue(SpecialUserIndex::SERVER_MAIN_THREAD);
	int32 retVal = Constants::ServerMainReturnValues::normalExit;
	bool exitCommand = false;
	while(!exitCommand){
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		if(msgQueue->hasMessage()){
			auto msg = msgQueue->popMessage();
			switch(msg.id){
				case Constants::ServerMainMsg::exitMainThread:
					socket.stop();
					retVal = msg.data;
					logger.pushLog("Server stopped", Logger::AlertLevel::Info, LOGGER_TAG);
					exitCommand = true;
					break;
				default:
					logger.pushLog("ServerMain received an unknown message, code:" + ArmyAnt::String(msg.id) + ", data:" + int64(msg.data), Logger::AlertLevel::Warning, LOGGER_TAG);
			}
		}
	}

	// 5. �˳�ʱ������Դ
	auto uninitRes = modulesUninitialization();
	logger.pushLog("Program over", Logger::AlertLevel::Info, LOGGER_TAG);
	return retVal;
}

UserSessionManager&ServerMain::getUserSessionManager(){
	return sessionMgr;
}

MessageQueueManager&ServerMain::getMessageQueueManager(){
	return msgQueueMgr;
}

int32 ServerMain::parseConfig(){
	ArmyAnt::File configJson;
	// �������ļ�
	bool openRes = configJson.Open(Constants::SERVER_CONFIG_FILE_PATH);
	if(!openRes){
		return Constants::ServerMainReturnValues::openConfigFileFailed;
	}
	// ��ȡ��������
	auto jsonFileLen = configJson.GetLength();
	char* buf = new char[jsonFileLen + 20];
	memset(buf, 0, jsonFileLen + 20);
	bool readRes = configJson.Read(buf);
	configJson.Close();
	if(!readRes){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		return Constants::ServerMainReturnValues::parseConfigJsonFailed;
	}
	// ���л�������
	auto json = ArmyAnt::JsonUnit::create(buf);
	auto pJo = dynamic_cast<ArmyAnt::JsonObject*>(json);
	if(pJo == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}

	// ����������ڴ�
	auto&jo = *pJo;
	auto jdebug = jo.getChild("debug");
	auto pjdebug = dynamic_cast<ArmyAnt::JsonBoolean*>(jdebug);
	if(pjdebug == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	debug = pjdebug->getBoolean();

	auto jport = jo.getChild("port");
	auto pjport = dynamic_cast<ArmyAnt::JsonNumeric*>(jport);
	if(pjport == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	port = pjport->getInteger();

	auto jlogPath = jo.getChild("logPath");
	auto logFilePath = dynamic_cast<ArmyAnt::JsonString*>(jlogPath);
	if(logFilePath == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	logger.setLogFile(logFilePath->getString());

	// �ļ���־ɸѡ����
	auto logFileLevel = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("logFileLevel"));
	if(logFileLevel->getString() == ArmyAnt::String("verbose")){
		logger.setFileLevel(Logger::AlertLevel::Verbose);
	} else	if(logFileLevel->getString() == ArmyAnt::String("debug")){
		logger.setFileLevel(Logger::AlertLevel::Debug);
	} else	if(logFileLevel->getString() == ArmyAnt::String("info")){
		logger.setFileLevel(Logger::AlertLevel::Info);
	} else	if(logFileLevel->getString() == ArmyAnt::String("import")){
		logger.setFileLevel(Logger::AlertLevel::Import);
	} else	if(logFileLevel->getString() == ArmyAnt::String("warning")){
		logger.setFileLevel(Logger::AlertLevel::Warning);
	} else	if(logFileLevel->getString() == ArmyAnt::String("error")){
		logger.setFileLevel(Logger::AlertLevel::Error);
	} else	if(logFileLevel->getString() == ArmyAnt::String("fatal")){
		logger.setFileLevel(Logger::AlertLevel::Fatal);
	} else{
		logger.setFileLevel(Logger::AlertLevel::Verbose);
	}

	// ����̨��־ɸѡ����
	auto logConsoleLevel = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("logConsoleLevel"));
	if(logConsoleLevel->getString() == ArmyAnt::String("verbose")){
		logger.setConsoleLevel(Logger::AlertLevel::Verbose);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("debug")){
		logger.setConsoleLevel(Logger::AlertLevel::Debug);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("info")){
		logger.setConsoleLevel(Logger::AlertLevel::Info);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("import")){
		logger.setConsoleLevel(Logger::AlertLevel::Import);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("warning")){
		logger.setConsoleLevel(Logger::AlertLevel::Warning);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("error")){
		logger.setConsoleLevel(Logger::AlertLevel::Error);
	} else	if(logConsoleLevel->getString() == ArmyAnt::String("fatal")){
		logger.setConsoleLevel(Logger::AlertLevel::Fatal);
	} else{
		logger.setConsoleLevel(Logger::AlertLevel::Import);
	}

	// DBProxy ��ַ
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
	auto jDBProxyLocalPort = dynamic_cast<ArmyAnt::JsonNumeric*>(jo.getChild("dbProxyLocalPort"));
	if(jDBProxyPort == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::ServerMainReturnValues::parseConfigJElementFailed;
	}
	dbLocalPort = uint16(jDBProxyLocalPort->getLong());


	ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
	ArmyAnt::JsonUnit::release(json);
	logger.pushLog("Config loading successful", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

int32 ServerMain::modulesInitialization(){
	// �������ݿ�������
	dbConnector.setEventCallback(std::bind(&ServerMain::onDBConnectorEvent, this, std::placeholders::_1, std::placeholders::_2));
	dbConnector.setReceiveCallback(std::bind(&ServerMain::onDBConnectorReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
	dbConnector.setWillReconnectWhenLost(true);
	auto ret = dbConnector.connect(*dbAddr, dbPort, dbLocalPort, false, 8192);
	while(!ret){
		logger.pushLog("DBproxy connected failed", Logger::AlertLevel::Error, LOGGER_TAG);
		ret = dbConnector.connect(*dbAddr, dbPort, dbLocalPort, false, 8192);
	}
	ArmyAnt::Fragment::AA_SAFE_DEL(dbAddr);

	logger.pushLog("All modules initialized successful", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

int32 ServerMain::modulesUninitialization(){
	dbConnector.disconnect();

	logger.pushLog("All modules uninitialized OK", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::ServerMainReturnValues::normalExit;
}

void ServerMain::onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content){
	switch(type){
		case SocketApplication::EventType::Connected:
			logger.pushLog("New client connected! client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Info, LOGGER_TAG);
			sessionMgr.createUserSession(clientIndex);
			break;
		case SocketApplication::EventType::Disconnected:
			logger.pushLog("Client disconnected! client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Info, LOGGER_TAG);
			sessionMgr.removeUserSession(clientIndex);
			break;
		case SocketApplication::EventType::SendingResponse:
			logger.pushLog("Sending to client responsed, client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Verbose, LOGGER_TAG);
			break;
		case SocketApplication::EventType::ErrorReport:
			logger.pushLog("Get socket error-report, client index: " + ArmyAnt::String(int64(clientIndex)) + ", content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
			break;
		case SocketApplication::EventType::Unknown:
			logger.pushLog("Get an unknown socket event, client index: " + ArmyAnt::String(int64(clientIndex)) + ", content: " + content, Logger::AlertLevel::Import, LOGGER_TAG);
			break;
		default:
			logger.pushLog("Get an unknown nomber of socket event, eventType number: " + ArmyAnt::String(int64(int8(type))) + ", client index: " + int64(clientIndex) + ", content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
	}
}

void ServerMain::onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body){
	logger.pushLog("Received from client index: " + ArmyAnt::String(int64(clientIndex)) + ", appid: " + int64(appid), Logger::AlertLevel::Verbose, LOGGER_TAG);
}

void ServerMain::onDBConnectorEvent(SocketClientApplication::EventType type, ArmyAnt::String content){
	switch(type){
		case SocketClientApplication::EventType::Connected:
			logger.pushLog("DBProxy connected!", Logger::AlertLevel::Info, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::LostServer:
			logger.pushLog("DBProxy server lost!", Logger::AlertLevel::Info, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::SendingResponse:
			logger.pushLog("Sending to DBProxy responsed", Logger::AlertLevel::Verbose, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::ErrorReport:
			logger.pushLog("Get DBProxy socket error-report, content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
			break;
		case SocketClientApplication::EventType::Unknown:
			logger.pushLog("Get an unknown socket event from DBProxy socket, content: " + content, Logger::AlertLevel::Import, LOGGER_TAG);
			break;
		default:
			logger.pushLog("Get an unknown nomber of socket event, eventType number: " + ArmyAnt::String(int64(int8(type))) + ", content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
	}
}

void ServerMain::onDBConnectorReceived(const MessageBaseHead & head, uint64 appid, int32 contentLength, int32 contentCode, void * body){
	logger.pushLog("Received from DBProxy, appid: " + int64(appid), Logger::AlertLevel::Verbose, LOGGER_TAG);
}


}
