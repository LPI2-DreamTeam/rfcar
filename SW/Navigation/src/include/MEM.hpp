#ifndef MEM_H
#define MEM_H

#include "main.hpp"

#include <inttypes.h>


namespace MEM {

	/*! @brief Blocking modes in read/write operations when the buffer is empty/full
	*
	*	In BLOCKING	mode the operation halts until the list is dead or ready to be read from / written to
	*	In NON_BLOCKING mode the operation is stopped immediatley, irrespective oof the state of completion
	*/
	typedef enum BlockingMode_T {
		NON_BLOCKING = 0, BLOCKING
	} BlockingMode;


	/*! @brief End positions for insertion of objects in lists 
	*
	*	FRONT refers to the first position
	*	BACK refers to the last position
	*/
	typedef enum Position_T {
		FRONT = 0, BACK
	} Position;


	/*! @brief Generic error codes
	*
	*	These codes convey a general message regarding the result of the lastly executed function.
	*	They update after each function call for all functions but the constructors, destructors and getLastError()
	*/
	typedef enum Error_T {
		OK = 0, DEAD, FULL, EMPTY, INVALID_SIZE, NOT_FOUND
	} Error;
}

#endif