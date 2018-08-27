#include <ArmyAnt.h>
#include <DBProxyMain.h>

#include <functional>

#include <DBProxyConstants.h>

using ArmyAntServer::Logger;

static const char* const LOGGER_TAG = "DBProxyMain";

namespace ArmyAntDBProxy{


DBProxyMain::DBProxyMain() :debug(false), port(0), msgQueue(nullptr), socket(), logger(), eventMgr(), msgQueueMgr(), mysqlBridge(){}

DBProxyMain::~DBProxyMain(){}

int32 DBProxyMain::main(){
	// 1. 读取配置
	auto parseRes = parseConfig();
	if(parseRes != Constants::DBProxyMainReturnValues::normalExit){
		return parseRes;
	}

	// 2. 初始化各个模块, 注意顺序
	auto initRes = modulesInitialization();
	if(initRes != Constants::DBProxyMainReturnValues::normalExit){
		return initRes;
	}

	// 3. 初始化并开启 socket TCP server
	socket.setEventCallback(std::bind(&DBProxyMain::onSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	socket.setReceiveCallback(std::bind(&DBProxyMain::onSocketReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	socket.start(port, 16384, false);
	logger.pushLog("DBProxy started", Logger::AlertLevel::Info, LOGGER_TAG);

	// 4. 开始监听主线程消息队列
	msgQueue = msgQueueMgr.createQueue(ArmyAntServer::SpecialUserIndex::DBPROXY_MAIN);
	int32 retVal = Constants::DBProxyMainReturnValues::normalExit;
	bool exitCommand = false;
	while(!exitCommand){
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		if(msgQueue->hasMessage()){
			auto msg = msgQueue->popMessage();
			switch(msg.id){
				case Constants::DBProxyMainMsg::exitMainThread:
					socket.stop();
					retVal = msg.data;
					logger.pushLog("DBProxy stopped", Logger::AlertLevel::Info, LOGGER_TAG);
					exitCommand = true;
					break;
				default:
					logger.pushLog("DBProxyMain received an unknown message, code:" + ArmyAnt::String(msg.id) + ", data:" + int64(msg.data), Logger::AlertLevel::Warning, LOGGER_TAG);
			}
		}
	}

	logger.pushLog("Program over", Logger::AlertLevel::Info, LOGGER_TAG);
	return retVal;
}

ArmyAntServer::MessageQueueManager&DBProxyMain::getMessageQueueManager(){
	return msgQueueMgr;
}

int32 DBProxyMain::parseConfig(){
	ArmyAnt::File configJson;
	// 打开设置文件
	bool openRes = configJson.Open(Constants::SERVER_CONFIG_FILE_PATH);
	if(!openRes){
		return Constants::DBProxyMainReturnValues::openConfigFileFailed;
	}
	// 读取设置内容
	auto jsonFileLen = configJson.GetLength();
	char* buf = new char[jsonFileLen + 20];
	memset(buf, 0, jsonFileLen + 20);
	bool readRes = configJson.Read(buf);
	configJson.Close();
	if(!readRes){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		return Constants::DBProxyMainReturnValues::parseConfigJsonFailed;
	}
	// 序列化设置项
	auto json = ArmyAnt::JsonUnit::create(buf);
	auto pJo = dynamic_cast<ArmyAnt::JsonObject*>(json);
	if(pJo == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::DBProxyMainReturnValues::parseConfigJElementFailed;
	}

	// 保存设置项到内存
	auto&jo = *pJo;
	auto jdebug = jo.getChild("debug");
	auto pjdebug = dynamic_cast<ArmyAnt::JsonBoolean*>(jdebug);
	if(pjdebug == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::DBProxyMainReturnValues::parseConfigJElementFailed;
	}
	debug = pjdebug->getBoolean();

	auto jport = jo.getChild("port");
	auto pjport = dynamic_cast<ArmyAnt::JsonNumeric*>(jport);
	if(pjport == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::DBProxyMainReturnValues::parseConfigJElementFailed;
	}
	port = pjport->getInteger();

	auto jlogPath = jo.getChild("logPath");
	auto logFilePath = dynamic_cast<ArmyAnt::JsonString*>(jlogPath);
	if(logFilePath == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return Constants::DBProxyMainReturnValues::parseConfigJElementFailed;
	}
	logger.setLogFile(logFilePath->getString());

	// 文件日志筛选级别
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

	// 控制台日志筛选级别
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


	ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
	ArmyAnt::JsonUnit::release(json);
	logger.pushLog("Config loading successful", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::DBProxyMainReturnValues::normalExit;
}

int32 DBProxyMain::modulesInitialization(){
	auto connRes = mysqlBridge.connect(mysqlUsername, mysqlPassword);
	if(!connRes){
		logger.pushLog("Connect to Mysql failed", Logger::AlertLevel::Fatal, LOGGER_TAG);
		return Constants::DBProxyMainReturnValues::moduleInitFailed;
	}

	logger.pushLog("All modules initialized successful", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::DBProxyMainReturnValues::normalExit;
}

int32 DBProxyMain::modulesUninitialization(){
	mysqlBridge.disconnect();

	logger.pushLog("All modules uninitialized OK", Logger::AlertLevel::Info, LOGGER_TAG);
	return Constants::DBProxyMainReturnValues::normalExit;
}


void DBProxyMain::onSocketEvent(ArmyAntServer::SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content){
	switch(type){
		case ArmyAntServer::SocketApplication::EventType::Connected:
			logger.pushLog("New client connected! client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Info, LOGGER_TAG);
			break;
		case ArmyAntServer::SocketApplication::EventType::Disconnected:
			logger.pushLog("Client disconnected! client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Info, LOGGER_TAG);
			break;
		case ArmyAntServer::SocketApplication::EventType::SendingResponse:
			logger.pushLog("Sending to client responsed, client index: " + ArmyAnt::String(int64(clientIndex)), Logger::AlertLevel::Verbose, LOGGER_TAG);
			break;
		case ArmyAntServer::SocketApplication::EventType::ErrorReport:
			logger.pushLog("Get socket error-report, client index: " + ArmyAnt::String(int64(clientIndex)) + ", content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
			break;
		case ArmyAntServer::SocketApplication::EventType::Unknown:
			logger.pushLog("Get an unknown socket event, client index: " + ArmyAnt::String(int64(clientIndex)) + ", content: " + content, Logger::AlertLevel::Import, LOGGER_TAG);
			break;
		default:
			logger.pushLog("Get an unknown nomber of socket event, eventType number: " + ArmyAnt::String(int64(int8(type))) + ", client index: " + int64(clientIndex) + ", content: " + content, Logger::AlertLevel::Warning, LOGGER_TAG);
	}
}

void DBProxyMain::onSocketReceived(const uint32 clientIndex, const ArmyAntServer::MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body){
	logger.pushLog("Received from client index: " + ArmyAnt::String(int64(clientIndex)) + ", appid: " + int64(appid), Logger::AlertLevel::Verbose, LOGGER_TAG);
}


}
