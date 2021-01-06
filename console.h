// console.h
//
// Platform dependant console functionality.
//
// TODO:
// Eventually, the implementation of these functions will (and has
// to) differ for unix and windows.

#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED 1


#include <stdlib.h>


namespace ui
{

namespace console
{

void clearScreen();
char getChar();

} // namespace console

} // namespace ui


#endif // CONSOLE_H_INCLUDED
