// ArmyAntServer.cpp: 定义控制台应用程序的入口点。
//

// 预编译头必须在最前
#include <stdafx.h>
// 标准库
#include <iostream>
// 头文件
#include <../inc/ServerMain.h>

int main()
{
	ArmyAntServer::ServerMain server;
	auto returnValue = server.main();
    return returnValue;
}

