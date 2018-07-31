#include <ArmyAnt.h>
#include <ServerMain.h>

#include <functional>

#include <ServerCoreConstants.h>

namespace ArmyAntServer{


ServerMain::ServerMain()
	:sessionMgr(*this){}

ServerMain::~ServerMain(){}

int32 ServerMain::main(){
	// 1. 读取配置
	auto parseRes = parseConfig();
	if(parseRes < 0){
		return parseRes;
	}

	// 2. 初始化各个模块, 注意顺序
	auto initRes = modulesInitialization();
	if(parseRes < 0){
		return initRes;
	}

	// 3. 初始化并开启 socket TCP server
	socket.setEventCallback(std::bind(&ServerMain::onSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	socket.setReceiveCallback(std::bind(&ServerMain::onSocketReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	socket.start(port, 16384, false);

	// 4. 开始监听主线程消息队列
	msgQueue = msgQueueMgr.createQueue(SpecialUserIndex::SERVER_MAIN_THREAD);
	while(true){
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		if(msgQueue->hasMessage()){
			auto msg = msgQueue->popMessage();
			// TODO : Resolve msg
		}
	}

	return 0;
}

UserSessionManager&ServerMain::getUserSessionManager(){
	return sessionMgr;
}

MessageQueueManager&ServerMain::getMessageQueueManager(){
	return msgQueueMgr;
}

int32 ServerMain::parseConfig(){
	ArmyAnt::File configJson;
	// 打开设置文件
	bool openRes = configJson.Open(Constants::SERVER_CONFIG_FILE_PATH);
	if(!openRes){
		return -1;
	}
	// 读取设置内容
	auto jsonFileLen = configJson.GetLength();
	char* buf = new char[jsonFileLen + 20];
	memset(buf, 0, jsonFileLen + 20);
	bool readRes = configJson.Read(buf);
	configJson.Close();
	if(!readRes){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		return -2;
	}
	// 序列化设置项
	auto json = ArmyAnt::JsonUnit::create(buf);
	auto pJo = dynamic_cast<ArmyAnt::JsonObject*>(json);
	if(pJo == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return -3;
	}

	// 保存设置项到内存
	auto&jo = *pJo;
	auto jdebug = jo.getChild("debug");
	auto pjdebug = dynamic_cast<ArmyAnt::JsonBoolean*>(jdebug);
	if(pjdebug == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return -3;
	}
	debug = pjdebug->getBoolean();

	auto jport = jo.getChild("port");
	auto pjport = dynamic_cast<ArmyAnt::JsonNumeric*>(jport);
	if(pjport == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return -3;
	}
	port = pjport->getInteger();

	auto jlogPath = jo.getChild("logPath");
	auto logFilePath = dynamic_cast<ArmyAnt::JsonString*>(jlogPath);
	if(logFilePath == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return -3;
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
	return 0;
}

int32 ServerMain::modulesInitialization(){


	return 0;
}


void ServerMain::onSocketEvent(SocketApplication::EventType type, const uint32 clientIndex, ArmyAnt::String content){
	// TODO
}
void ServerMain::onSocketReceived(const uint32 clientIndex, const MessageBaseHead&head, uint64 appid, int32 contentLength, int32 contentCode, void*body){
	// TODO
}

}
