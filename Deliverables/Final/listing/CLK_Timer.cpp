#include "CLK_Timer.hpp"

#include <string>
#include <chrono>
#include <iostream>

namespace CLK {

	Timer::Timer() : Timer((CLK::TimeElapsedCallback*)nullptr, nullptr) {

	}

	Timer::Timer(CLK::TimeElapsedCallback* time_elapsed_callback, void* callback_arg) : id(next_id), thread(std::string("Timer " + 
		std::to_string(next_id++)).c_str(), this->threadMethod, this, OS::Thread::StackSize::DONT_CARE, OS::Thread::Priority::REAL_TIME) {
		
		this->isr = time_elapsed_callback;
		this->stop_order = false;
		this->done = true;
		this->isr_arg = callback_arg;
	}

	void Timer::threadMethod(OS::Thread* thread, void* arg) {

		Timer* tmr_ptr = reinterpret_cast<Timer*>(arg);

		std::unique_lock<std::mutex> lock(tmr_ptr->mutex);

		auto start = std::chrono::steady_clock::now();

		while (1) {
			
			tmr_ptr->condition.wait_until(lock, start + std::chrono::milliseconds(tmr_ptr->counter), [tmr_ptr]() {return tmr_ptr->stop_order == true; });
			
			// Keep timestamp
			start = std::chrono::steady_clock::now();

			// Notify of the end of the timer countdown
			tmr_ptr->notification_tim_ov.notifyAll();

			// If the wait ended because of a stop order, the program must exit the loop
			if (tmr_ptr->stop_order) {
				tmr_ptr->stop_order = false;
				break;
			}

			// Run interrupt service routine
			(*tmr_ptr->isr)(tmr_ptr->isr_arg);

			// Notify of the end of the ISR
			tmr_ptr->notification_isr_done.notifyAll();

			// Reload counter
			tmr_ptr->counter = tmr_ptr->auto_reload;
		}

		tmr_ptr->done = true;
	};

	void Timer::start() {
		
		std::unique_lock<std::mutex> lock(this->mutex);
		last_error = OK;
		done = false;
		thread.run();
	}

	void Timer::stop() {

		std::unique_lock<std::mutex> lock(this->mutex);
		stop_order = true;
		last_error = OK;
	}

	void Timer::waitNotification(bool isr_done, bool tim_ov) {
		
		if (tim_ov)
			notification_tim_ov.wait(); 

		if (isr_done)
			notification_isr_done.wait();

		last_error = OK;
	}

}