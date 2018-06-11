#ifndef MESSAGE_QUEUE_MANAGER_H_20180607
#define MESSAGE_QUEUE_MANAGER_H_20180607

#include <map>

#include <AADefine.h>

#include "SpecialUserIndexDefined.h"
#include "MessageQueue.h"

namespace ArmyAntServer{

class MessageQueueManager{
public:
	MessageQueueManager();
	~MessageQueueManager();

public:
	const MessageQueue * getQueue(int32 userId)const;
	bool pushMessageTo(int32 userId, Message msg);
	MessageQueue * createQueue(int32 userId);
	bool removeQueue(int32 userId);

private:
	std::map<int32, MessageQueue> queueList;

	AA_FORBID_ASSGN_OPR(MessageQueueManager);
	AA_FORBID_COPY_CTOR(MessageQueueManager);
};

} // namespace ArmyAntServer

#endif // MESSAGE_QUEUE_MANAGER_H_20180607
