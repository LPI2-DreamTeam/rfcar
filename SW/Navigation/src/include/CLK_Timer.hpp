#ifndef _CLK_TIMER_H
#define _CLK_TIMER_H

#include "OS_Notification.hpp"
#include "OS_Thread.hpp"

namespace CLK {

	class Timer {
	
	private:

		static uint32_t next_id;

	public: 	// Public methods
		
		Timer();
		
		Timer(CLK::TimeElapsedCallback* time_elapsed_callback);
			
		void start();
			
		void stop();
			
		/**
		 * @param arr
		 */
		CLK::Error setAutoReload(uint32_t arr);
			
		/**
		 * @param cnt
		 */
		CLK::Error setCounter(uint32_t cnt);
			
		bool isDone();
			
		/**
		 * @param tim_ov
		 * @param isr_done
		 */
		void waitNotification(bool isr_done = false, bool tim_ov = true);
		
		void threadMethod(OS::Thread* thread);

		CLK::Error getLastError();
		

	private: 	// Private members

		Error last_error;
		bool done;
		bool stop_order;
		uint32_t id;

		OS::Notification notification_tim_ov;
		OS::Notification notification_isr_done;

		CLK::TimeElapsedCallback* isr;
		OS::Thread thread;

		uint32_t counter;
		uint32_t auto_reload;

#ifdef _LINUX_
		std::mutex mutex;					/*! Mutex for use with std::unique_lock in managing accesses and operations */
		std::condition_variable condition;	/*! Condition variable for use in waits (see waiting_data: bool) */
#endif
	};
}

#endif //_CLK_TIMER_H