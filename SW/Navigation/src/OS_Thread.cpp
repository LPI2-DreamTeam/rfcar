#include "OS_Thread.hpp"
#include "CLK.hpp"

#ifdef _LINUX_

#include <iostream>
#include <string>
#include <cstring>


namespace OS {

	const Thread::ID empty_thread_id = Thread::ID();

	Thread::Thread(const char name[20], Method method, StackSize stack_size, Priority priority) :

		self(&(Thread::main_method), this), parent_id(std::this_thread::get_id()) {

		start = false;
		this->method = method;
		std::memcpy(this->name, name, 20);

		// In windows we ignore priority and stack size altogether
		this->priority = priority;
		this->stack_size = stack_size;

		last_timestamp = std::chrono::system_clock::now();
	}

	Thread::~Thread() {
		self.detach();

	}
	
	Thread::ID Thread::ownID() {
		return self.get_id();

	}

	void Thread::keepCurrentTimestamp() {
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			last_timestamp = std::chrono::system_clock::now();
	}

	void Thread::sleepUntilElapsed(uint32_t time_ms) {
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			std::this_thread::sleep_until(this->last_timestamp + CLK::TimeUnit(time_ms));
	}

	void Thread::sleepFor(uint32_t time_ms) {
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			std::this_thread::sleep_for(static_cast<CLK::TimeUnit>(time_ms));
	}


	void Thread::run() {
		// Check if it is the parent thread calling
		if (Thread::currentThreadID() == parent_id)
			start = true;
	}


	Thread::ID Thread::currentThreadID() {
		return std::this_thread::get_id();
	}

	void Thread::join() {
		if (self.joinable())
			self.join();
	}

	bool Thread::joinable() {
		return self.joinable();
	}

	void Thread::detach() {
		self.detach();
		parent_id = empty_thread_id;
	}

}

#endif	 // _LINUX_