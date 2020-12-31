// console.h
//
// Platform dependant console functionality.
//
// Separate in this header+source to isolate conditional compilation.

#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED 1

#if
#include <stdlib.>

namespace console
{

void clearScreen()
{
  system("clear");
}


} // namespace console


#endif // CONSOLE_H_INCLUDED
