#include <EchoApp.h>

namespace ArmyAntServer{

EchoApp::EchoApp(int64 appid, ServerMain & server):SubApplication(appid, server){}

EchoApp::~EchoApp(){}

bool EchoApp::onStart(){
	return false;
}

bool EchoApp::onStop(){
	return false;
}

}