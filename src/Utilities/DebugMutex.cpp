#include <DebugMutex.h>

#ifdef AAS_MUTEX_DEBUG
#include <iostream>
#endif

namespace ArmyAntServer{

#ifdef AAS_MUTEX_DEBUG

std::mutex Mutex::managerLock;
std::map<int32, Mutex*> Mutex::idManager;

int32 Mutex::addMutex(Mutex* mutex){
	managerLock.lock();
	for(int i = 0; i < AA_INT32_MAX; ++i){
		if(idManager.find(i) == idManager.end()){
			idManager.insert(std::make_pair(i, mutex));
			managerLock.unlock();
			return i;
		}
	}
	managerLock.unlock();
	return -1;
}

Mutex::Mutex() :mutex(), mutexId(addMutex(this)), isLogging(true){
	managerLock.lock();
	std::cout << "Mutex " << mutexId << " " << tag.c_str() << " created in thread " << std::this_thread::get_id() << std::endl;
	managerLock.unlock();
}

Mutex::~Mutex(){
	managerLock.lock();
	std::cout << "Mutex " << mutexId << " " << tag.c_str() << mutexId << " destroyed in thread " << std::this_thread::get_id() << std::endl;
	idManager.erase(idManager.find(mutexId));
	managerLock.unlock();
}

void Mutex::lock(){
	if(isLogging){
		managerLock.lock();
		std::cout << "Mutex " << mutexId << " " << tag.c_str() << mutexId << " to lock in thread " << std::this_thread::get_id() << std::endl;
		managerLock.unlock();
	}
	mutex.lock();
	if(isLogging){
		managerLock.lock();
		std::cout << "Mutex " << mutexId << " " << tag.c_str() << mutexId << " locked in thread " << std::this_thread::get_id() << std::endl;
		managerLock.unlock();
	}
}

bool Mutex::try_lock(){
	if(isLogging){
		managerLock.lock();
		std::cout << "Mutex " << mutexId << " " << tag.c_str() << mutexId << " try-locking in thread " << std::this_thread::get_id() << std::endl;
		managerLock.unlock();
	}
	return mutex.try_lock();
}

void Mutex::unlock(){
	if(isLogging){
		managerLock.lock();
		std::cout << "Mutex " << mutexId << " " << tag.c_str() << mutexId << " unlocked in thread " << std::this_thread::get_id() << std::endl;
		managerLock.unlock();
	}
	mutex.unlock();
}

#endif // AAS_MUTEX_DEBUG

namespace MutexHelper{

void setMutexLogOff(Mutex& mutex){
#ifdef AAS_MUTEX_DEBUG
	mutex.isLogging = false;
#endif
}

void setMutexLogOn(Mutex& mutex){
#ifdef AAS_MUTEX_DEBUG
	mutex.isLogging = true;
#endif
}

void setMutexLogTag(Mutex& mutex, const char*tag){
#ifdef AAS_MUTEX_DEBUG
	mutex.tag = tag;
#endif
}

}

}

