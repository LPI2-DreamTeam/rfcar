#include "Mutex.h"

#if defined(__PLATFORM_WINDOWS__)

Mutex::Mutex() : self(), owner(empty_thread_id) {}

Mutex::~Mutex() {}


bool Mutex::tryLock() {
	if (self.try_lock()) {
		owner = Thread::currentThreadID();
		return true;
	}

	return false;
}

void Mutex::lock() {
	self.lock();
	owner = Thread::currentThreadID();
}


void Mutex::unlock() {
	if (owner == Thread::currentThreadID()) {
		owner = empty_thread_id;
		self.unlock();
	}
}

#endif