#pragma once

#define __WINDOWS__
// #define __LINUX__

#ifdef __LINUX__
#ifdef __WINDOWS__
#undef __WINDOWS__
#endif
#endif

#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN
#endif