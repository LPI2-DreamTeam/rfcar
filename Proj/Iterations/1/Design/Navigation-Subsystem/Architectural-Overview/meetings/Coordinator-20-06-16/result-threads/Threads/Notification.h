#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "main.h"

#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
#include <condition_variable>
#endif


class Notification {

private:	// Private members
	
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
	std::condition_variable condition;
#endif
	bool awaiting_notification;

public:		// Public methods

	Notification();

	~Notification();

	inline void notifyOne() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		condition.notify_one();
#endif
	}


	inline void wait() {
#if defined(__PLATFORM_WINDOWS__) || defined(__PLATFORM_LINUX__)
		std::mutex mut;
  		std::unique_lock<std::mutex> lock(mut);
		condition.wait(lock);
#endif
	}
};


#endif