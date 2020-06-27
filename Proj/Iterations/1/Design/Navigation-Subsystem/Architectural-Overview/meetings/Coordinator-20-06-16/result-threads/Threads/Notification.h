#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "main.h"

#if defined(__PLATFORM_WINDOWS__)
#include <condition_variable>

using NativeNotification = std::condition_variable;

#endif


class Notification {

private:	// Private members
	
	NativeNotification condition;
	bool awaiting_notification;

public:		// Public methods

	Notification();

	~Notification();

	void notifyOne();

	void notifyAll();

	void wait();
};


#endif