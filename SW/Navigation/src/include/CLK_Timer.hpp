#ifndef _CLK_TIMER_H
#define _CLK_TIMER_H

#include "OS_Notification.hpp"
#include "OS_Thread.hpp"

namespace CLK {

	class Timer {
	
	private:

		static uint32_t next_id;
	
	private:

		OS::Thread::Method thread_method;

	public: 	// Public methods
		
		Timer();

		static CLK::Time currentTime();
			
		void start();
			
		void stop();
			
		/**
		 * @param arr
		 */
		void setAutoReload(Time arr);
			
		/**
		 * @param cnt
		 */
		void setCounter(Time cnt);
			
		bool isDone();
			
		/**
		 * @param tim_ov
		 * @param isr_done
		 */
		void waitNotification(bool tim_ov, bool isr_done);
			
		CLK::Error getLastError();
		

	private: 	// Private members

		uint32_t id;
		CLK::TimeElapsedCallback isr;
		OS::Thread thread;
		OS::Notification notification_tim_ov;
		OS::Notification notification_isr_done;
		bool done;
		Error last_error;
	};
}

#endif //_CLK_TIMER_H