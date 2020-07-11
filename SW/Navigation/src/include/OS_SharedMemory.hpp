#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "main.hpp"
#include "OS_Mutex.hpp"
#include "OS_Thread.hpp"

#ifdef _LINUX_

#include <list>

namespace OS {

	template<class T>
	class SharedMemory {


	private:	// Private members

		// Linked list of associated threads. Implemented as an STL structure in Windows/Linux for simplicity of operation
		std::list<Thread*> threads;

		// Mutex that will help manage access to the memory
		Mutex mutex;

		// Memory region to be managed
		T* self;

		// Thread that has last been authorized to access to the memory
		Thread::ID authorized_thread;

	public:		// Public methods 

		// Constructor/default constructor
		SharedMemory(T* memory = nullptr) {
			authorized_thread = Thread::ID();
			self = memory;
		}

		// Destructor
		~SharedMemory() {}

		// Add thread to list of threads with access to the memory
		inline void addThread(Thread* thread) {
			threads.push_back(thread);
			threads.unique();
		}

		// Request access to memory. This has to be done by a thread in order to access the memory and when it is done, only release() 
		// may free it for another one
		inline bool grab() {
			mutex.lock();
			if (authorized_thread == empty_thread_id) {
				for (auto thread : threads) {
					if (thread->ownID() == Thread::currentThreadID()) {
						// mutex.lock();
						authorized_thread = Thread::currentThreadID();
						return true;
					}
				}
			}
			mutex.unlock();

			return false;
		}

		// grab() but in a non-blocking fashion
		inline bool grab_non_blocking() {
			for (auto thread : threads) {
				if (thread->ownID() == authorized_thread) {
					if (mutex.tryLock()) {
						authorized_thread = Thread::currentThreadID();
						return true;
					}
					break;
				}
			}
			// return false if the thread is not authorized, the mutex is locked or the function is not implemented for the specific platform
			return false;
		}

		// Releases the memory to be used by another thread. Only the authorized thread can do this.
		inline bool release() {
			if (Thread::currentThreadID() == authorized_thread) {
				authorized_thread = empty_thread_id;
				mutex.unlock();
				return true;
			}
			return false;
		}

		// Controlled access to the memory
		inline T* data() {

			// Return immediately if no thread has requested access
			if (authorized_thread == empty_thread_id)
				return nullptr;

			// If memory access has been requested and granted to the current thread, return the pointer to the protected memory
			if (authorized_thread == Thread::currentThreadID())
				return self;

			// Returns nullptr if the current thread has no privilege over the protected memory
			return nullptr;
		}


	};

}

#endif	 // _LINUX_

#endif // !SHARED_MEMORY_H