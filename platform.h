// platform.h
//
// Preprocessor mamcros distinguuishing between supported platforms.


#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED 1


#ifdef __linux__
#define TENK_LINUX
#else // FOXME not very reliable
#define TENK_WINDOWS
#endif

// TODO Needs implementatino if used
#define TENK_GCC


#endif // PLATFORM_H_INCLUDED
