// platform.h
//
// Preprocessor mamcros distinguuishing between supported platforms.


#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED 1


#ifdef __linux__
#define TENK_LINUX
#else
#define TENK_WINDOWS
#endif

#define TENK_GCC


#endif // PLATFORM_H_INCLUDED
