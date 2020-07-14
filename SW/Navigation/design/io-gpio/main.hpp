#ifndef _MAIN_H
#define _MAIN_H

#include <inttypes.h>

// #define _DEBUG_
#define _LINUX_

#define loop while(1)


typedef union number_t {
	float _float;
	uint32_t _uint32;
	uint8_t _uint8_arr[4];
} number;

#endif