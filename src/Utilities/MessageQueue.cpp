#include <MessageQueue.h>

namespace ArmyAntServer{

MessageQueue::MessageQueue(int32 userIndex, MessageQueueManager & mgr)
	:mgr(mgr), userIndex(userIndex){}

MessageQueue::MessageQueue(MessageQueue&&moved)
	: mgr(moved.mgr), userIndex(moved.userIndex){

}

MessageQueue::~MessageQueue(){}

MessageQueueManager & MessageQueue::getManager(){
	return mgr;
}

int32 MessageQueue::getUserIndex(){
	return userIndex;
}

bool MessageQueue::pushMessage(Message msg){
	msgQueue.push(msg);
	return true;
}

bool MessageQueue::hasMessage(){
	return !msgQueue.empty();
}
Message MessageQueue::popMessage(){
	auto ret = msgQueue.front();
	msgQueue.pop();
	return ret;
}

Message MessageQueue::getMessage()const{
	return msgQueue.front();
}


} // namespace ArmyAntServer
