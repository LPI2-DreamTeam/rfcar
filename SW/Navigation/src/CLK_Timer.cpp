#include "CLK_Timer.hpp"



namespace CLK {

	uint32_t Timer::next_id = 0;

	Timer::Timer() : Timer((CLK::TimeElapsedCallback*)nullptr) {

	}

	Timer::Timer(CLK::TimeElapsedCallback* time_elapsed_callback) : id(next_id), thread(std::string("Timer " + 
		std::to_string(next_id++)).c_str(), (OS::Thread::Method)(&CLK::Timer::threadMethod), OS::Thread::StackSize::DONT_CARE, OS::Thread::Priority::REAL_TIME) {
		
		this->isr = time_elapsed_callback;
		this->stop_order = false;
		this->done = false;
	}

	void Timer::threadMethod(OS::Thread* thread) {

		std::unique_lock<std::mutex> lock(this->mutex);

		auto start = std::chrono::system_clock::now();

		while(1) {
			using namespace std::chrono_literals;
			condition.wait_until(lock, start + counter*1ms, [this](){return this->stop_order == true;});

			// Keep timestamp
			start = std::chrono::system_clock::now();
			
			// Notify of the end of the timer countdown
			notification_tim_ov.notifyAll();

			// If the wait ended because of a stop order, the program must exit the loop
			if (stop_order) {
				stop_order = false;
				break;
			}

			// Run interrupt service routine
			(*isr)(nullptr);

			// Notify of the end of the ISR
			notification_isr_done.notifyAll();

			// Reload counter
			counter = auto_reload;
		}

		done = true;
		
	}

	/**
	 * @return void
	 */
	void Timer::start() {
		
		std::unique_lock<std::mutex> lock(this->mutex);

		thread.run();
	}

	/**
	 * @return void
	 */
	void Timer::stop() {

		std::unique_lock<std::mutex> lock(this->mutex);
		stop_order = true;
	}

	/**
	 * @param arr
	 * @return void
	 */
	CLK::Error Timer::setAutoReload(uint32_t arr) {
		std::unique_lock<std::mutex> lock(this->mutex);
		
		last_error = OK;
		
		if (!done)
			last_error = STILL_RUNNING;

		else 
			auto_reload = arr;

		return last_error;
	}

	/**
	 * @param cnt
	 * @return void
	 */
	CLK::Error Timer::setCounter(uint32_t cnt) {
		std::unique_lock<std::mutex> lock(this->mutex);
		
		last_error = OK;

		if (!done) 
			last_error = STILL_RUNNING;

		else 
			counter = cnt;

		return last_error;
	}

	/**
	 * @return bool
	 */
	bool Timer::isDone() {
		return done;
	}

	/**
	 * @param tim_ov
	 * @param isr_done
	 * @return void
	 */
	void Timer::waitNotification(bool isr_done, bool tim_ov) {
		
		if (tim_ov)
			notification_tim_ov.wait(); 

		if (isr_done)
			notification_isr_done.wait();

		last_error = OK;
	}

	/**
	 * @return Error
	 */
	Error Timer::getLastError() {
		return last_error;
	}
}