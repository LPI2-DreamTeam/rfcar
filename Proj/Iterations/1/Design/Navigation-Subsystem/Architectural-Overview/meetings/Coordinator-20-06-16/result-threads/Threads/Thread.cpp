#include "Thread.h"

#if defined(__PLATFORM_WINDOWS__)
#include <iostream>
#include <string>


const ThreadID empty_thread_id = ThreadID();

Thread::Thread(const char name[20], ThreadMethod method, ThreadStackSize stack_size, ThreadPriority priority) :

	self(&(Thread::main_method), this), parent_id(std::this_thread::get_id()) {

	start = false;
	this->method = method;
	memcpy(this->name, name, 20);

	// In windows we ignore priority and stack size altogether
	this->priority = priority;
	this->stack_size = stack_size;

	last_timestamp = std::chrono::system_clock::now();
}

Thread::~Thread() {
	self.detach();

}

ThreadID Thread::ownID() {
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
		std::this_thread::sleep_until(this->last_timestamp + TimeUnit(time_ms));
}

void Thread::sleepFor(uint32_t time_ms) {
	// Check if it is the thread itself calling
	if (std::this_thread::get_id() == self.get_id())
		std::this_thread::sleep_for(static_cast<TimeUnit>(time_ms));
}


void Thread::run() {
	// Check if it is the parent thread calling
	if (Thread::currentThreadID() == parent_id)
		start = true;
}


ThreadID Thread::currentThreadID() {
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

#endif