#include <MessageQueue.h>

namespace ArmyAntServer{

MessageQueue::MessageQueue(int32 userIndex, MessageQueueManager & mgr)
	:mgr(mgr), userIndex(userIndex), msgQueue(), mutex(){
	MutexHelper::setMutexLogOff(mutex);
}

MessageQueue::MessageQueue(MessageQueue&&moved)
	: mgr(moved.mgr), userIndex(moved.userIndex), msgQueue(), mutex(){
	while(!moved.msgQueue.empty()){
		msgQueue.push(moved.msgQueue.front());
		moved.msgQueue.pop();
	}
	MutexHelper::setMutexLogOff(mutex);
}

MessageQueue::~MessageQueue(){}

MessageQueueManager & MessageQueue::getManager(){
	return mgr;
}

int32 MessageQueue::getUserIndex(){
	return userIndex;
}

bool MessageQueue::pushMessage(Message msg){
	mutex.lock();
	msgQueue.push(msg);
	mutex.unlock();
	return true;
}

bool MessageQueue::hasMessage(){
	mutex.lock();
	auto ret = !msgQueue.empty();
	mutex.unlock();
	return ret;
}
Message MessageQueue::popMessage(){
	mutex.lock();
	auto ret = msgQueue.front();
	msgQueue.pop();
	mutex.unlock();
	return ret;
}

Message MessageQueue::getMessage()const{
	return msgQueue.front();
}


} // namespace ArmyAntServer
