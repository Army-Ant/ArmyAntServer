#include <subApplication.h>

namespace ArmyAntServer{

SubApplication::SubApplication(int64 appid, ServerMain & server):appid(appid), server(server), msgQueue(nullptr){}

SubApplication::~SubApplication(){}

MessageQueue * SubApplication::getMessageQueue(){
	return msgQueue;
}

}
