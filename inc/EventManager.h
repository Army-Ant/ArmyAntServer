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
	typedef std::function<void(int32 userId, LocalEventData*data)> LocalEventCallback;
	typedef std::function<void(int32 extendVerstion, int32 conversationCode, int32 userId, void*message, int32 length)> NetworkResponseCallback;

public:
	EventManager(UserSessionManager& sessionMgr, Logger&logger);
	~EventManager();

public:

	bool addListenerForLocalEvent(int32 code, int32 userId, LocalEventCallback cb);
	bool addGlobalListenerForLocalEvent(std::string tag, int32 code, LocalEventCallback cb);
	bool removeListenerForLocalEvent(int32 code, int32 userId);
	bool removeGlobalListenerForLocalEvent(std::string tag, int32 code);
	bool dispatchLocalEvent(int32 code, int32 userId, LocalEventData*data);

	bool addListenerForNetworkResponse(int32 code, int32 userId, NetworkResponseCallback cb);
	bool addGlobalListenerForNetworkResponse(std::string tag, int32 code, NetworkResponseCallback cb);
	bool removeListenerForNetworkResponse(int32 code, int32 userId);
	bool removeGlobalListenerForNetworkResponse(std::string tag, int32 code);
	bool dispatchNetworkResponse(int32 code, int32 userId, int32 conversationCode, int32 conversationStepIndex, ArmyAntMessage::System::ConversationStepType conversationStepType, void*message, int32 messageLen);

public:
	static int32 getProtobufMessageCode(google::protobuf::Message*message);
	template <class T> static int32 getProtobufMessageCode();
	template <class T> static T* castMessage(void*message);

private:
	UserSessionManager& sessionMgr;
	Logger&logger;
	std::map<int32, std::map<std::string, LocalEventCallback>> localEventListenerList;
	std::map<int32, std::map<std::string, NetworkResponseCallback>> networkListenerList;

private:
	AA_FORBID_ASSGN_OPR(EventManager);
	AA_FORBID_COPY_CTOR(EventManager);
};

template <class T> static int32 EventManager::getProtobufMessageCode(){
	return T::descriptor()->options().GetExtension(msg_code);
}

template <class T> static T* EventManager::castMessage(void*message){
	return dynamic_cast<T*>(static_cast<google::protobuf::Message*>(message));
}


} // namespace ArmyAntServer

#endif // EVENT_MANAGER_H_20180606
