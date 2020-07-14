#include "OS_Notification.hpp"


#ifdef _LINUX_

namespace OS {

	Notification::Notification() : awaiting_notification(false) {}


	Notification::~Notification() {}


	void Notification::notifyOne() {
		condition.notify_one();
	}

	void Notification::notifyAll() {
		condition.notify_all();
	}


	void Notification::wait() {
		std::mutex mut;
		std::unique_lock<std::mutex> lock(mut);
		condition.wait(lock);
	}
}

#endif	 // _LINUX_