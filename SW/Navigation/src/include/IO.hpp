#ifndef _IO_H
#define _IO_H

#include "main.hpp"

namespace IO {
	
	typedef void (*ConvCpltCallback)(number);

	typedef struct Config_T {
		uint32_t update_period;
		uint32_t no_of_conversions;
		ConvCpltCallback conv_cplt_callback;
	} Config;

	typedef enum Error_t {
		OK=0
	} Error;

}

#endif //_IO_H