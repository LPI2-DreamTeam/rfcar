#include "Thread.h"

#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
#include <iostream>
#include <string>
#endif

const ThreadID empty_thread_id = ThreadID();

Thread::Thread(const char name[20], ThreadMethod method, ThreadStackSize stack_size, ThreadPriority priority) :
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
	self(&(Thread::main_method), this), parent_id(std::this_thread::get_id()) {
#endif 

	start = false;
	this->method = method;
	memcpy(this->name, name, 20);

#if defined(__PLATFORM_WINDOWS__)

	// In windows we ignore priority and stack size altogether
	this->priority = priority;
	this->stack_size = stack_size;

	last_timestamp = std::chrono::system_clock::now();

#elif defined(__PLATFORM_LINUX__)

	// TODO: Linux implementation

#endif
}

Thread::~Thread() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
	self.detach();
#endif
}
