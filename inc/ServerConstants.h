#ifndef SERVER_CONSTANTS_H_20180607
#define SERVER_CONSTANTS_H_20180607

namespace ArmyAntServer{

namespace Constants{

const char* SERVER_CONFIG_FILE_PATH = "../res/ConfigJson/ServerMainConfig.json";

struct ServerMainMsg{
	static const int32 exitMainThread = 1001;
	static const int32 dbProxyNeedReconnect = 1002;
};

struct ServerMainReturnValues{
	static const int32 serverStartFailed = -5;
	static const int32 moduleInitFailed = -4;
	static const int32 parseConfigJElementFailed = -3;
	static const int32 parseConfigJsonFailed = -2;
	static const int32 openConfigFileFailed = -1;
	static const int32 normalExit = 0;
};

struct ServerMainAppid{
	static const int32 simpleEchoApp = 1001;
};

}

}

#endif // SERVER_CONSTANTS_H_20180607
