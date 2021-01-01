#include "console.h"

#include <iostream>  // dbg/testing

#include <ncurses.h>  // L


namespace ui
{

namespace console
{

void clearScreen()
{
  // Currnetly unix only
  auto ret = system("clear");  // L
  if (ret == -1)
    throw std::runtime_error("Clear Screen L failed");
}

char getChar()
{
  // This does not look efficient, but we want to block for key press anyway
	initscr();			/* Start curses mode 		  */
	auto c = getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

  return c;
}

namespace // testing
{

int testClearScreen()
{
  std::cout << "Testing clearScreen(): PRess Enter to clear." << std::endl;
  std::cin.ignore();
  clearScreen();
  std::cout << "Screen cleared." << std::endl;
  std::cin.ignore();
  return 0;
}

int testGetChar()
{
  std::cout << "Testing getChar(): type now" << std::endl;
  auto c = console::getChar();
  std::cout << "You typed:" << c << std::endl;
  std::cin.ignore();
  return 0;
}

#ifndef NDEBUG
int i =
  +testClearScreen()
  +testGetChar()
  ;
#endif  // NDEBUG

} // namespace

} // namespace console

} // namespace ui
