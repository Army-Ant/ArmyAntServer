// ArmyAntServer.cpp: 定义控制台应用程序的入口点。
//

// 预编译头必须在最前
#include <stdafx.h>

// 头文件
#include <ServerMain.h>
#include <WindowsResourcesManager.h>

int main()
{
	auto res = WindowsResourcesManager::getInstance();

	ArmyAntServer::ServerMain server;
	auto returnValue = server.main();

	WindowsResourcesManager::releaseInstance();
    return returnValue;
}

