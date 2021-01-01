#include "actor.h"

#include "console.h"

namespace ui
{

void
HumanActor::operator()(Game_t const& game)
{
  (void)game; // FIXME
  std::cout << "Awaiting order" << std::endl;
  auto c = console::getChar();
  std::cout << "Your order: " << c << std::endl;
  // Bind Game_t member to arguments but do not specify object
}

} // namespace ui
