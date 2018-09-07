// ArmyAntDBProxy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdafx.h>

#include <DBProxyMain.h>

int main()
{
	ArmyAntDBProxy::DBProxyMain dbProxy;
	auto returnValue = dbProxy.main();


#ifdef OS_WINDOWS
	exit(returnValue);
#endif
	return returnValue;
}
