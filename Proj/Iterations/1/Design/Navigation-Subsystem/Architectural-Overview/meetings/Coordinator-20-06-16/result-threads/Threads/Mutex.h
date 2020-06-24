#ifndef MUTEX_H
#define MUTEX_H

#include "main.h"
#include "Thread.h"

#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)

#include <mutex>

using NativeMutex = std::mutex;

#endif 

class Mutex {


private:	// Private members

	NativeMutex self;
	ThreadID owner;

public:		// Public methods

	// Constructor
	Mutex();

	// Destructor
	~Mutex();

	// Lock mutex if not locked
	inline bool tryLock() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		if (self.try_lock()) {
			owner = Thread::currentThreadID();
			return true;
		}

		return false;
#endif
	}

	// Lock mutex
	inline void lock() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		self.lock();
		owner = Thread::currentThreadID();
#endif
	}

	// Unlock mutex
	inline void unlock() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		if (owner == Thread::currentThreadID()) {
			owner = empty_thread_id;
			self.unlock();
		}
#endif
	}
};


#endif // !MUTEX_H