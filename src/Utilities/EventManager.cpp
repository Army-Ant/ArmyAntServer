#include <EventManager.h>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>

namespace ArmyAntServer{

EventManager::EventManager()
	:enabled(false), localCB(nullptr), netCB(nullptr){

}

EventManager::~EventManager(){

}

void EventManager::setEnable(bool enable){
	enabled = enable;
}

bool EventManager::isEnabled() const{
	return enabled;
}

bool EventManager::setLocalEventCallback(LocalEventCallback cb){
	localCB = cb;
	return true;
}

bool EventManager::setNetworkResponseCallback(NetworkResponseCallback cb){
	netCB = cb;
	return true;
}

bool EventManager::registerLocalEvent(int32 code, int32 userIndex){
	auto listeners = localListener.find(code);
	if(listeners == localListener.end()){
		localListener.insert(std::make_pair(code, std::vector<int32>()));
		listeners = localListener.find(code);
	}
	if(std::find(listeners->second.begin(), listeners->second.end(), userIndex) != listeners->second.end()){
		return false;
	}
	listeners->second.push_back(userIndex);
	return true;
}

bool EventManager::unregisterLocalEvent(int32 code, int32 userIndex){
	auto listeners = localListener.find(code);
	if(listeners == localListener.end()){
		localListener.insert(std::make_pair(code, std::vector<int32>()));
		return false;
	}
	auto cb = std::find(listeners->second.begin(), listeners->second.end(), userIndex);
	if(cb == listeners->second.end()){
		return false;
	}
	listeners->second.erase(cb);
	return true;
}

bool EventManager::dispatchLocalEvent(int32 code, int32 userIndex, LocalEventData*data){
	if(localCB == nullptr)
		return false;
	auto listeners = localListener.find(code);
	if(listeners == localListener.end())
		return false;
	if(std::find(listeners->second.begin(), listeners->second.end(), userIndex) == listeners->second.end())
		return false;
	localCB(code, userIndex, data);
	return true;
}

bool EventManager::registerNetworkResponse(int32 code, int32 userIndex){
	auto listeners = networkListener.find(code);
	if(listeners == networkListener.end()){
		networkListener.insert(std::make_pair(code, std::vector<int32>()));
		listeners = networkListener.find(code);
	}
	if(std::find(listeners->second.begin(), listeners->second.end(), userIndex) != listeners->second.end()){
		return false;
	}
	listeners->second.push_back(userIndex);
	return true;
}

bool EventManager::unregisterNetworkResponse(int32 code, int32 userIndex){
	auto listeners = networkListener.find(code);
	if(listeners == networkListener.end()){
		networkListener.insert(std::make_pair(code, std::vector<int32>()));
		return false;
	}
	auto cb = std::find(listeners->second.begin(), listeners->second.end(), userIndex);
	if(cb == listeners->second.end()){
		return false;
	}
	listeners->second.erase(cb);
	return true;
}

bool EventManager::dispatchNetworkResponse(int32 code, int32 userIndex, google::protobuf::Message*data){
	if(netCB == nullptr)
		return false;
	auto listeners = networkListener.find(code);
	if(listeners == networkListener.end())
		return false;
	if(std::find(listeners->second.begin(), listeners->second.end(), userIndex) == listeners->second.end())
		return false;
	netCB(code, userIndex, data);
	return true;
}

int32 EventManager::getProtobufMessageCode(google::protobuf::Message * message){
	return message->GetDescriptor()->options().GetExtension(msg_code);
}



} // namespace ArmyAntServer
