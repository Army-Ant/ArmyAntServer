#ifndef MESSAGE_QUEUE_H_20180607
#define MESSAGE_QUEUE_H_20180607

#include <queue>

#include <AADefine.h>


namespace ArmyAntServer{
class MessageQueueManager;

struct Message{
	int32 id;
	int32 data;
	void* pdata;
};

class MessageQueue{
public:
	MessageQueue(MessageQueue&&moved);
	~MessageQueue();

public:
	MessageQueueManager & getManager();
	int32 getUserIndex();

public:
	bool pushMessage(Message msg);
	bool hasMessage();
	Message popMessage();
	Message getMessage()const;

private:
	MessageQueueManager & mgr;
	int32 userIndex;
	std::queue<Message> msgQueue;

	MessageQueue(int32 userIndex, MessageQueueManager & mgr);
	AA_FORBID_ASSGN_OPR(MessageQueue);
	AA_FORBID_COPY_CTOR(MessageQueue);

	friend class MessageQueueManager;
};

} // namespace ArmyAntServer


#endif // MESSAGE_QUEUE_H_20180607
