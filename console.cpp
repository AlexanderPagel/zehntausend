#include "console.h"
#include "platform.h" // needed for includes

#include <stdexcept>
#include <thread>
#include <chrono> // std::chrono::milliseconds
#ifdef TESTING
#include <iostream>  // dbg/testing
#endif  // TESTING

#ifdef TENK_LINUX
#include <ncurses.h>
#endif // TENK_LINUX
#ifdef TENK_WINDWOS
#include <windows.h>
#endif // TENK_WINDOWS


namespace ui
{

namespace console
{

void clearScreen()
{
#ifdef TENK_LINUX
  // Currnetly unix only
  auto ret = system("clear");  // Linux
  if (ret == -1)
    throw std::runtime_error("Clear Screen failed");
#endif // TENK_LINUX
#ifdef TENK_WINDOWS
  System("cls");
#endif // TENK_WINDOWS
}

char getChar()
{
#ifdef TENK_LINUX
  // This does not look efficient, but we want to block for key press anyway
  initscr();      /* Start curses mode       */
  timeout(-1);  // Blocking input
  cbreak();     // NO line-wise buffering
  auto c = getch();      /* Wait for user input */
  endwin();      /* End curses mode      */

  return c;
#endif // TENK_LINUX
#ifdef TENK_WINDOWS
  auto c = getch();
  return c;
#endif // TENK_WINDOWS
}

void sleep(int milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
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

int testSleep()
{
  std::cout << "Testing sleep(): Press [ENTER] to sleep 1 second." << std::endl;
  std::cin.ignore();
  sleep(1000);
  std::cout << "Slept for 1 second." << std::endl;
  std::cout << "Press [ENTER] to continue." << std::endl;
  return 0;
}

int test_nothing() { return 0; }
int i = test_nothing()
  +testClearScreen()
  +testGetChar()
  +testSleep()
  ;

} // namespace
#endif  // TESTING

} // namespace console

} // namespace ui
