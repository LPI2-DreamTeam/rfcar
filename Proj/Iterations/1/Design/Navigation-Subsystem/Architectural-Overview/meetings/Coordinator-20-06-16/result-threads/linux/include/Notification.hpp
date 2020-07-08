#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "main.hpp"

#ifdef _LINUX_

#include <condition_variable>

typedef std::condition_variable native_notification;

#endif	 // _LINUX_


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