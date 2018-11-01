#ifndef DEBUG_MUTEX_H_20181101
#define DEBUG_MUTEX_H_20181101

#include <mutex>

/**
	This file is made to resolve mutex-lock problems
*/

#ifdef DEBUG
//#define AAS_MUTEX_DEBUG
#endif

#ifdef AAS_MUTEX_DEBUG
#include <AADefine.h>
#include <map>
#endif

namespace ArmyAntServer{
#ifdef AAS_MUTEX_DEBUG
class Mutex;
#else
typedef std::mutex Mutex;
#endif

namespace MutexHelper{

void setMutexLogOff(Mutex& mutex);
void setMutexLogOn(Mutex& mutex);
void setMutexLogTag(Mutex& mutex, const char*tag);

}

#ifdef AAS_MUTEX_DEBUG

class Mutex{
public:
	Mutex();
	~Mutex();

public:
	void lock();
	bool try_lock();
	void unlock();

private:
	std::mutex mutex;
	int32 mutexId;
	bool isLogging;
	std::string tag;

private:
	static std::mutex managerLock;
	static std::map<int32, Mutex*> idManager;
	static int32 addMutex(Mutex* mutex);

	friend void MutexHelper::setMutexLogOff(Mutex&);
	friend void MutexHelper::setMutexLogOn(Mutex&);
	friend void MutexHelper::setMutexLogTag(Mutex& mutex, const char*tag);
};

#endif

}

#endif // DEBUG_MUTEX_H_20181101
