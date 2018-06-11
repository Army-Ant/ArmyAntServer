#include <UserSession.h>

namespace ArmyAntServer{

UserSession::UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr)
	:index(index), msgQueue(msgQueue), mgr(mgr){}

UserSession::UserSession(UserSession&&moved)
	: index(moved.index), msgQueue(moved.msgQueue), mgr(moved.mgr){}

UserSession::~UserSession(){}

UserSessionManager&UserSession::getManager(){
	return mgr;
}

int32 UserSession::getUserIndex(){
	return index;
}

}
