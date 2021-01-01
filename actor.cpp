#include "actor.h"

#include "console.h"

namespace ui
{

// Reading stdin:
// 1-6 put aside given die
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
  // Bind Game_t member to arguments but do not specify object
}

} // namespace ui
