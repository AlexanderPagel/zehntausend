#include "actor.h"

#include <iostream> // dbg

#include "console.h"

namespace ui
{

HumanActor::RespondFunc_t
HumanActor::classifyInput(char input)
{
  // Interface keys
  if ('1' <= input && input <= '6') return &HumanActor::respondToDieInput;
  if (input == 'a')                 return &HumanActor::respondToAll;
  if (input == 'r')                 return &HumanActor::respondToRoll;
  if (input == 'f')                 return &HumanActor::respondToFinish;
  if (input == 'l')                 return &HumanActor::respondToQuit;

  // Unmapped keys
  return &HumanActor::respondToInvalid;
}

void
HumanActor::operator()()
{
  // TODO
  // 1. get human input
  // 2. check validity
  // 3. Allow corrections if invalid
  // 4. hand to UI class

  std::cout << "Awaiting order" << std::endl;
  auto c = console::getChar();
  std::cout << "Your order: " << c << std::endl;
  std::cin.ignore();
  // Bind Game_t member to arguments but do not specify object

  // ----------------------------------------------------------

  // Get 1 character of user input
  char input{console::getChar()};

  // Handle user input
  auto&& responseFunction{classifyInput(input)};
  (this->*responseFunction)(input);

  lastInput = input;
}

} // namespace ui
