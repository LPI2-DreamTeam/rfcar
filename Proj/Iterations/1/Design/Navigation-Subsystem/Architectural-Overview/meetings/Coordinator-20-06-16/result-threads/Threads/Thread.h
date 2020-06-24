#ifndef THREAD_H
#define THREAD_H

#include "main.h"

////////////////////////////// PLATFORM-SPECIFIC //////////////////////////////

#if defined(__PLATFORM_WINDOWS__)

#include <thread>
#include <functional>
#include <windows.h>
#include <processthreadsapi.h>
#include <inttypes.h>

using ThreadID = std::thread::id;
using NativeThread = std::thread;
using Time = std::chrono::time_point<std::chrono::system_clock>;
using TimeUnit = std::chrono::milliseconds;

extern const ThreadID empty_thread_id;

typedef enum ThreadPriority_T {
	IDLE = 1,
	LOW = 6,
	BELOW_NORMAL = 7,
	NORMAL = 8,
	ABOVE_NORMAL = 9,
	HIGH = 10,
	REAL_TIME = 15
} ThreadPriority;


#elif defined(__PLATFORM_LINUX__)

// TODO: includes

using ThreadID = std::thread::id;
using NativeThread = std::thread;
using Time = std::chrono::time_point < std::chrono::system_clock>;
using TimeUnit = std::chrono::milliseconds;

typedef enum ThreadPriority {
	// TODO: ThreadPriority
	IDLE = ,
	LOW = ,
	BELOW_NORMAL = ,
	NORMAL = ,
	ABOVE_NORMAL = ,
	HIGH = ,
	REAL_TIME = 
};
 
#endif


////////////////////////////// PLATFORM-AGNOSTIC //////////////////////////////

typedef enum ThreadStackSize_T {
	DONT_CARE = 0,
	BYTES_128 = 128,
	BYTES_256 = 256,
	BYTES_384 = 384,
	BYTES_512 = 512,
	BYTES_640 = 640,
	BYTES_768 = 768,
	BYTES_896 = 896,
	BYTES_1024 = 1024,
	BYTES_2048 = 2048,
	BYTES_4096 = 4096,
	BYTES_8192 = 8192
} ThreadStackSize;


class Thread {

////////////////////////////// PLATFORM-AGNOSTIC //////////////////////////////

	typedef void(*ThreadMethod)(Thread*);

private:	// Private members

	ThreadID parent_id;
	NativeThread self;

	// Thread's priority for use in preferential scheduling in certain platforms
	ThreadPriority priority;

	// Thread stack size for use in certain platforms
	ThreadStackSize stack_size;

	// Timestamp used for using sleep functions as timing base for the thread's execution
	Time last_timestamp;

	// signal forthe main method to start execution
	bool start;

	// Thread method
	ThreadMethod method;


public:		// Public members

	// Short and informative name. It is public because a 'get' method would expose it completely anyway.
	char name[20];


private:	// Private methods

	inline static void main_method(Thread* thread) {
		
		while (!(thread->start && thread->method != nullptr));
		
		thread->method(thread);
	}

public:		// Public methods

	// Constructor
	Thread(const char name[20], ThreadMethod method, ThreadStackSize stack_size = ThreadStackSize::DONT_CARE, 
		ThreadPriority priority = ThreadPriority::NORMAL);

	// Destructor
	~Thread();

	// Returns the id of the parent thread
	inline ThreadID parentID() {
		return parent_id;
	}

	// Returns the threads own id
	inline ThreadID ownID() {
		return self.get_id();
	}

	// Keep current timestamp as base for the next sleepUntil()
	inline void keepCurrentTimestamp() {
	#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			last_timestamp = std::chrono::system_clock::now();
	#endif
	}

	// Sleep until (last_timestamp + time_ms) ms
	inline void sleepUntilElapsed(uint32_t time_ms) {
	#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			std::this_thread::sleep_until(this->last_timestamp + TimeUnit(time_ms));
	#endif
	}

	// Sleep for time_ms ms
	inline void sleepFor(uint32_t time_ms) {
	#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		// Check if it is the thread itself calling
		if (std::this_thread::get_id() == self.get_id())
			std::this_thread::sleep_for(static_cast<TimeUnit>(time_ms));
	#endif
	}

	// Used by parent thread to set the current thread running
	inline void run() {
		// Check if it is the parent thread calling
		if (Thread::currentThreadID() == parent_id)
			start = true;
	}

	inline static ThreadID currentThreadID() {
	#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		return std::this_thread::get_id();
	#endif
	}


////////////////////////////// PLATFORM-SPECIFIC //////////////////////////////

#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
	
	// ThreadPool will need to access join() and joinable() in its destructor but it doesn't make much sense having those methods 
	// declared as public in a platform-agnostic environment, as in some platforms the program might be a never-ending loop
	friend class ThreadPool;
	friend int main(int argc, char * argv[]);

public:	// methods

	// Returns when the thread execution has completed
	inline void join() {
		if (self.joinable())
			self.join();
	}

	// Returns whether the thread object is joinable.
	inline bool joinable() {
		return self.joinable();
	}

	// Detaches this thread from the calling thread
	inline void detach() {
		self.detach();
		parent_id = empty_thread_id;
	}
#endif

};


#endif // !THREAD_H