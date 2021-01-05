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
  // FIXME Does not seem to work. Try init and end in main and never leaving
  //       ncurses mode.

  // This does not look efficient, but we want to block for key press anyway
  initscr();      /* Start curses mode       */
  timeout(-1);  // Blocking input
  cbreak();     // NO line-wise buffering
  auto c = getch();      /* Wait for user input */
  endwin();      /* End curses mode      */

  return c;
}

#ifdef TESTING
namespace
{

int testClearScreen()
{
  std::cout << "Testing clearScreen(): Press Enter to clear." << std::endl;
  std::cin.ignore();
  clearScreen();
  std::cout << "Screen cleared. Press Enter to continue." << std::endl;
  std::cin.ignore();
  std::cout << "Continuing after clearing screen..." << std::endl;
  return 0;
}

int testGetChar()
{
  std::cout << "Testing getChar(): Press any key." << std::endl;
  auto c = console::getChar();
  std::cout << "You typed:" << c << std::endl;
  std::cout << "Press Enter to continue." << std::endl;
  std::cin.ignore();
  return 0;
}

int test_nothing() { return 0; }
int i = test_nothing()
  +testClearScreen()
  +testGetChar()
  ;

} // namespace
#endif  // TESTING

} // namespace console

} // namespace ui
