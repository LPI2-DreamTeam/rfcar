#ifndef THREAD_H
#define THREAD_H
#include "main.h"

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

typedef void(*ThreadMethod)(Thread*);

#endif // defined(__PLATFORM_WINDOWS__)

extern const ThreadID empty_thread_id;

class Thread {

#if defined(__PLATFORM_WINDOWS__)
	// Enumeration of possible thread priosities
	typedef enum ThreadPriority_T {
		IDLE = 1,
		LOW = 6,
		BELOW_NORMAL = 7,
		NORMAL = 8,
		ABOVE_NORMAL = 9,
		HIGH = 10,
		REAL_TIME = 15
	} ThreadPriority;
#endif

	// Enumeration of possible stack sizes
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
	ThreadID ownID();

	// Keep current timestamp as base for the next sleepUntil()
	void keepCurrentTimestamp();

	// Sleep until (last_timestamp + time_ms) ms
	void sleepUntilElapsed(uint32_t time_ms);

	// Sleep for time_ms ms
	void sleepFor(uint32_t time_ms);

	// Used by parent thread to set the current thread running
	void run();

	static ThreadID currentThreadID();


public:	// methods

	// Returns when the thread execution has completed
	void join();

	// Returns whether the thread object is joinable.
	bool joinable();

	// Detaches this thread from the calling thread
	void detach();

};


#endif // !THREAD_H