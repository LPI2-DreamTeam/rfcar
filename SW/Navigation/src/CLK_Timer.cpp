#include "CLK_Timer.hpp"



namespace CLK {

	uint32_t Timer::next_id = 0;

	Timer::Timer() : id(next_id), thread(std::string("Timer " + std::to_string(next_id++)).c_str(), thread_method, OS::Thread::StackSize::DONT_CARE, OS::Thread::Priority::REAL_TIME) {

	}

	void thread_method(OS::Thread* thread) {

	}

	/**
	 * @return Time
	 */
	Time Timer::currentTime() {
		return Time();
	}

	/**
	 * @return void
	 */
	void Timer::start() {
		return;
	}

	/**
	 * @return void
	 */
	void Timer::stop() {
		return;
	}

	/**
	 * @param arr
	 * @return void
	 */
	void Timer::setAutoReload(Time arr) {
		return;
	}

	/**
	 * @param cnt
	 * @return void
	 */
	void Timer::setCounter(Time cnt) {
		return;
	}

	/**
	 * @return bool
	 */
	bool Timer::isDone() {
		return false;
	}

	/**
	 * @param tim_ov
	 * @param isr_done
	 * @return void
	 */
	void Timer::waitNotification(bool tim_ov, bool isr_done) {
		return;
	}

	/**
	 * @return Error
	 */
	Error Timer::getLastError() {
		return OK;
	}
}