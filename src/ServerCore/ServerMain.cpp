#include <ArmyAnt.h>
#include <ServerMain.h>

#include <functional>

#include <ServerCoreConstants.h>

namespace ArmyAntServer{


ServerMain::ServerMain()
	:sessionMgr(*this){}

ServerMain::~ServerMain(){}

int32 ServerMain::main(){
	// 1. ��ȡ����
	auto parseRes = parseConfig();
	if(parseRes < 0){
		return parseRes;
	}

	// 2. ��ʼ������ģ��, ע��˳��
	auto initRes = modulesInitialization();
	if(parseRes < 0){
		return initRes;
	}

	// 3. ��ʼ�������� socket TCP server
	socket.setEventCallback(std::bind(&ServerMain::onSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	socket.setReceiveCallback(std::bind(&ServerMain::onSocketReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
	socket.start(port, 16384, false);

	// 4. ��ʼ�������߳���Ϣ����

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
	// �������ļ�
	bool openRes = configJson.Open(Constants::SERVER_CONFIG_FILE_PATH);
	if(!openRes){
		return -1;
	}
	// ��ȡ��������
	char* buf = new char[configJson.GetLength() + 20];
	bool readRes = configJson.Read(buf);
	configJson.Close();
	if(!readRes){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		return -2;
	}
	// ���л�������
	auto json = ArmyAnt::JsonUnit::create(buf);
	auto pJo = dynamic_cast<ArmyAnt::JsonObject*>(json);
	if(pJo == nullptr){
		ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
		ArmyAnt::JsonUnit::release(json);
		return -3;
	}
	// ����������ڴ�
	auto&jo = *pJo;
	debug = dynamic_cast<ArmyAnt::JsonBoolean*>(jo.getChild("debug"))->getBoolean();
	port = dynamic_cast<ArmyAnt::JsonNumeric*>(jo.getChild("port"))->getInteger();
	auto logFilePath = dynamic_cast<ArmyAnt::JsonString*>(jo.getChild("logPath"));
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


	ArmyAnt::Fragment::AA_SAFE_DELALL(buf);
	ArmyAnt::JsonUnit::release(json);
	return 0;
}

int32 ServerMain::modulesInitialization(){


	return 0;
}

}
