/**
 * Project Untitled
 */


#ifndef _CLK_TIMER_H
#define _CLK_TIMER_H

namespace CLK {

	class Timer {

	public: 	// Public methods
		
		static Time currentTime();
			
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
			
		Error getLastError();
		

	private: 	// Private members

		TimeElapsedCallback isr;
		OS::Thread thread;
		OS::Notification notification_tim_ov;
		OS::Notification notification_isr_done;
		bool done;
		Error last_error;
	};
}

#endif //_CLK_TIMER_H