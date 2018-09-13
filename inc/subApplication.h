#ifndef SUB_APPLICATION_H_20180913
#define SUB_APPLICATION_H_20180913

#include <AADefine.h>

namespace ArmyAntServer{
class ServerMain;
class MessageQueue;

class SubApplication{
public:
	SubApplication(int64 appid, ServerMain&server);
	virtual ~SubApplication();

public:
	virtual bool onStart() = 0;
	virtual bool onStop() = 0;

public:
	virtual MessageQueue*getMessageQueue();

protected:
	const int64 appid;
	ServerMain&server;
	MessageQueue* msgQueue;

private:
	AA_FORBID_ASSGN_OPR(SubApplication);
	AA_FORBID_COPY_CTOR(SubApplication);
};

}

#endif // SUB_APPLICATION_H_20180913
