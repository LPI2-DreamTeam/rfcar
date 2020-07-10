#include "Mutex.hpp"

#ifdef _LINUX_

#include <cstring>

namespace OS {

	Mutex::Mutex() : self(), owner(empty_thread_id) {}

	Mutex::Mutex(const Mutex& other_mutex) {
	
		memcpy(&(this->self), &(other_mutex.self), sizeof(this->self));
		this->owner = other_mutex.owner;
	}

	Mutex::~Mutex() {
		self.unlock();
	}


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


	Mutex::Native& Mutex::native() {

		return self;
	}
}

#endif	// _LINUX_