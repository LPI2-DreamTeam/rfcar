#ifndef _CLK_H
#define _CLK_H


#include <chrono>

namespace CLK {

	typedef std::chrono::time_point<std::chrono::system_clock> Time;
	typedef std::chrono::milliseconds TimeUnit;

	typedef void *(TimeElapsedCallback)(void*);

	typedef enum Error_T {
		OK=0, NOT_CONFIG
	} Error;
}

#endif //_CLK_H