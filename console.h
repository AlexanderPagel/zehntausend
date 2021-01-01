// console.h
//
// Platform dependant console functionality.

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
