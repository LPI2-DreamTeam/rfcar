#include "Notification.h"


#if defined(__PLATFORM_WINDOWS__)

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

#endif