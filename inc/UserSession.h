#ifndef USER_SESSION_H_20180607
#define USER_SESSION_H_20180607

#include <AADefine.h>
#include <AALog.h>
#include <map>
#include <functional>
#include <thread>
#include <DebugMutex.h>
#include <EventManager.h>

namespace ArmyAntServer{
class UserSessionManager;
class MessageQueue;
class SocketApplication;

class UserSession{
public:
	UserSession(UserSession&&moved);
	~UserSession();

public:    // Outer data
	UserSessionManager & getManager();
	int32 getUserIndex();
	void*getUserData()const;
	void setUserData(void*data);

public:
	void sendNetwork(int32 extendVersion, int64 appid, int32 conversationCode, ArmyAntMessage::System::ConversationStepType conversationStepType, google::protobuf::Message*content);

public:
	void dispatchLocalEvent(int32 code, LocalEventData* data);
	void dispatchNetworkEvent(int32 extendVerstion, int32 conversationCode, int32 code, void * data, int32 length);

public:
	void onUpdate();

private:
	// Outer data
	int32 index;
	MessageQueue&msgQueue;
	UserSessionManager&mgr;
	SocketApplication&socketSender;
	int32 senderIndex;
	void*userdata;

	bool threadEnd;
	std::thread updateThread;
	Mutex ioMutex;

	// Event data
	std::map<int32, EventManager::LocalEventCallback> localEventListenerList;
	std::map<int32, EventManager::NetworkResponseCallback> networkListenerList;
	std::map<std::string, EventManager::DisconnectCallback> disconnectCallback;
	std::map<int32, int32> conversationWaitingList;

	// unused private constructor
	UserSession(int32 index, MessageQueue&msgQueue, UserSessionManager&mgr, SocketApplication&socketSender, int32 senderIndex);
	AA_FORBID_ASSGN_OPR(UserSession);
	AA_FORBID_COPY_CTOR(UserSession);
	friend class UserSessionManager;
	friend class EventManager;
};

}// namespace ArmyAntServer


#endif // USER_SESSION_H_20180607
