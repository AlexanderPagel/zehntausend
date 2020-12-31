#include "console.h"

#include <iostream>  // dbg/testing

namespace ui
{

namespace console
{

void clearScreen()
{
  // Currnetly unix only
  system("clear");
}

namespace // testing
{

int testClearScreen()
{
  std::cout << "Test" << std::endl;
  std::cin.ignore();
  clearScreen();
  std::cout << "Test2" << std::endl;
  std::cin.ignore();
  return 0;
}

#ifndef NDEBUG
static int i = testClearScreen();
#endif  // NDEBUG

} // namespace

} // namespace console

} // namespace ui
