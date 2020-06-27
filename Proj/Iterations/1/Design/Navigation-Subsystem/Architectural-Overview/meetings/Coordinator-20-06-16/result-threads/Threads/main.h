#pragma once

#define __PLATFORM_WINDOWS__
// #define __PLATFORM_LINUX__

#ifdef __PLATFORM_LINUX__
#ifdef __PLATFORM_WINDOWS__
#undef __PLATFORM_WINDOWS__
#endif
#endif
