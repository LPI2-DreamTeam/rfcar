#ifndef MUTEX_H
#define MUTEX_H

#include "main.hpp"
#include "Thread.hpp"

#ifdef _LINUX_

#include <mutex>

typedef std::mutex native_mutex;

#endif 	 // _LINUX_


namespace OS {

	class Mutex {

		typedef native_mutex Native;
	
	private:	// Private members

		Native self;
		Thread::ID owner;

	public:		// Public methods

		// Constructor
		Mutex();

		// Destructor
		~Mutex();

		// Lock mutex if not locked
		bool tryLock();

		// Lock mutex
		void lock();

		// Unlock mutex
		void unlock();
	};
}


#endif // !MUTEX_H