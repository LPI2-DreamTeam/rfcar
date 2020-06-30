#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "main.h"

#if defined(__PLATFORM_WINDOWS__)
#include <condition_variable>

typedef std::condition_variable native_notification;

#endif


namespace OS {

	class Notification {

	public:

		typedef native_notification Native;

	private:	// Private members

		Native condition;
		bool awaiting_notification;

	public:		// Public methods

		Notification();

		~Notification();

		void notifyOne();

		void notifyAll();

		void wait();
	};
}


#endif