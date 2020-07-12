#ifndef COM_H
#define COM_H

#include "main.hpp"

namespace COM {

	/*! @brief Communication protocols */
	typedef enum Protocol_T {
		INTERFACE=0, SERIAL, BLUETOOTH
	} Protocol;

	/*! @brief Communication roles */
	typedef enum Role_T {
		VIRTUAL=0, CLIENT, SERVER
	} Role;

	/*! @brief Generic error codes
	*
	*	These codes convey a general message regarding the result of the lastly executed function. 
	*	They update after each function call for all functions but the constructors, destructors and getLastError()
	*/
	typedef enum Error_T {
		OK=0, INVALID_CONFIG, DEAD, NOT_CONNECTED, READ_FAIL, WRITE_FAIL, CLOSE_FAIL, OPEN_FAIL, ACCEPT_FAIL, ALREADY_OPEN, ALREADY_CLOSED, INVALID_PORT
	} Error;

}

#endif