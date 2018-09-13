#ifndef EVENT_MANAGER_H_20180606
#define EVENT_MANAGER_H_20180606

#include <functional>
#include <map>

#include <google/protobuf/message.h>
#include <ArmyAntMessage/Common/base.pb.h>
#include <ArmyAntMessage/System/SocketHead.pb.h>

#include <AADefine.h>

namespace ArmyAntServer{
class UserSessionManager;
class Logger;

// local event data must be inherited from this class
// network response will defaultly inherited from google::protobuf::message
class LocalEventData{
public:
	LocalEventData(int32 code);
	~LocalEventData();

private:
	int32 code;
};

class EventManager{
public:
	typedef std::function<void(LocalEventData*data)> LocalEventCallback;
	typedef std::function<void(google::protobuf::Message*message)> NetworkResponseCallback;

public:
	EventManager(UserSessionManager& sessionMgr, Logger&logger);
	~EventManager();

public:

	bool addListenerForLocalEvent(int32 code, int32 userId, LocalEventCallback cb);
	bool removeListenerForLocalEvent(int32 code, int32 userId);
	bool dispatchLocalEvent(int32 code, int32 userId, LocalEventData*data);

	bool addListenerForNetworkResponse(int32 code, int32 userId, NetworkResponseCallback cb);
	bool removeListenerForNetworkResponse(int32 code, int32 userId);
	bool dispatchNetworkResponse(int32 code, int32 userId, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message*message);

public:
	static int32 getProtobufMessageCode(google::protobuf::Message*message);
	template <class T> static int32 getProtobufMessageCode();

private:
	UserSessionManager& sessionMgr;
	Logger&logger;

private:
	AA_FORBID_ASSGN_OPR(EventManager);
	AA_FORBID_COPY_CTOR(EventManager);
};

template <class T> static int32 EventManager::getProtobufMessageCode(){
	T::descriptor()->options().GetExtension(msg_code);
}


} // namespace ArmyAntServer

#endif // EVENT_MANAGER_H_20180606
