// ArmyAntServer.cpp: 定义控制台应用程序的入口点。
//

// 预编译头必须在最前
#include <stdafx.h>

// 头文件
#include <ServerMain.h>
#ifdef OS_WINDOWS
#include <WindowsResourcesManager.h>
#endif

int main()
{
#ifdef OS_WINDOWS
	auto res = WindowsResourcesManager::getInstance();
#endif

	ArmyAntServer::ServerMain server;
	auto returnValue = server.main();

#ifdef OS_WINDOWS
	WindowsResourcesManager::releaseInstance();
#endif
    
    return returnValue;
}
