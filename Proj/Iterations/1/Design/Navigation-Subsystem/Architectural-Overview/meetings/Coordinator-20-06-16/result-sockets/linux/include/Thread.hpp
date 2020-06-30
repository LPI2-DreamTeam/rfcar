#ifndef THREAD_H
#define THREAD_H
#include "main.hpp"

#if defined(__WINDOWS__)

#include <thread>
#include <functional>
#include <windows.h>
#include <processthreadsapi.h>
#include <inttypes.h>

namespace OS {

	typedef std::chrono::time_point<std::chrono::system_clock> Time;
	typedef std::chrono::milliseconds TimeUnit;

	class Thread {

	public:

		typedef void(*Method)(Thread*);

		typedef std::thread::id ID;
		typedef std::thread Native;

		// Enumeration of possible thread priosities
		typedef enum Priority_T {
			IDLE = 1,
			LOW = 6,
			BELOW_NORMAL = 7,
			NORMAL = 8,
			ABOVE_NORMAL = 9,
			HIGH = 10,
			REAL_TIME = 15
		} Priority;

		// Enumeration of possible stack sizes
		typedef enum StackSize_T {
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
		} StackSize;


	private:	// Private members

		ID parent_id;
		Native self;

		// Thread's priority for use in preferential scheduling in certain platforms
		Priority priority;

		// Thread stack size for use in certain platforms
		StackSize stack_size;

		// Timestamp used for using sleep functions as timing base for the thread's execution
		Time last_timestamp;

		// signal forthe main method to start execution
		bool start;

		// Thread method
		Method method;


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
		Thread(const char name[20], Method method, StackSize stack_size = StackSize::DONT_CARE,
			Priority priority = Priority::NORMAL);

		// Destructor
		~Thread();

		// Returns the id of the parent thread
		inline ID parentID() {
			return parent_id;
		}

		// Returns the threads own id
		ID ownID();

		// Keep current timestamp as base for the next sleepUntil()
		void keepCurrentTimestamp();

		// Sleep until (last_timestamp + time_ms) ms
		void sleepUntilElapsed(uint32_t time_ms);

		// Sleep for time_ms ms
		void sleepFor(uint32_t time_ms);

		// Used by parent thread to set the current thread running
		void run();

		static ID currentThreadID();


	public:	// methods

		// Returns when the thread execution has completed
		void join();

		// Returns whether the thread object is joinable.
		bool joinable();

		// Detaches this thread from the calling thread
		void detach();

	};

	extern const Thread::ID empty_thread_id;
}


#endif // defined(__PLATFORM_WINDOWS__)

#endif // !THREAD_H