#include <MessageQueueManager.h>

namespace ArmyAntServer{

MessageQueueManager::MessageQueueManager(){

}

MessageQueueManager::~MessageQueueManager(){

}

const MessageQueue* MessageQueueManager::getQueue(int32 userId) const{
	auto finded = queueList.find(userId);
	if(finded == queueList.end())
		return nullptr;
	auto&ret = finded->second;
	return  &ret;
}

bool MessageQueueManager::pushMessageTo(int32 userId, Message msg){
	auto finded = queueList.find(userId);
	if(finded == queueList.end())
		return false;
	return finded->second.pushMessage(msg);
}

MessageQueue* MessageQueueManager::createQueue(int32 userId){
	auto finded = queueList.find(userId);
	if(finded != queueList.end())
		return nullptr;
	queueList.insert(std::make_pair(userId, MessageQueue(userId, *this)));
	auto&ret = queueList.find(userId)->second;
	return &ret;
}

bool MessageQueueManager::removeQueue(int32 userId){
	auto finded = queueList.find(userId);
	if(finded == queueList.end())
		return false;
	queueList.erase(finded);
	return true;
}

} // namespace ArmyAntServer