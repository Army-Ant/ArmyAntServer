#ifndef DBPROXY_CONSTANTS_H_20180827
#define DBPROXY_CONSTANTS_H_20180827

namespace ArmyAntServer{

namespace Constants{

const char* DB_PROXY_CONFIG_FILE_PATH = "../res/ConfigJson/DBProxyConfig.json";

struct DBProxyMainMsg{
	static const int32 exitMainThread = 1001;
};

struct DBProxyMainReturnValues{
	static const int32 serverStartFailed = -5;
	static const int32 moduleInitFailed = -4;
	static const int32 parseConfigJElementFailed = -3;
	static const int32 parseConfigJsonFailed = -2;
	static const int32 openConfigFileFailed = -1;
	static const int32 normalExit = 0;
};

}

}

#endif // DBPROXY_CONSTANTS_H_20180827
