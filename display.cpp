#include "display.h"

#include <array>
#include <ostream>
#include <iomanip>

#include "ui.h"

namespace ui
{

namespace
{

constexpr std::string_view constLines[] =
{
  {"     |    Dice:    1   2   3   4   5   6             1 never changing default line"}
};

} // namespace

void
Display::drawPlayerLine(Game_t::Player p)
{
  os << "Player " << p << ":";
  os << std::setw(5) << ui.getPoints(p) << std::endl;
}

void
Display::drawConstantLine(ConstLineIdx l)
{
  assert(unsigned(l) < std::size(constLines));
  os << constLines[int(l)] << std::endl;
}

void
Display::draw()
{

}

} // namespace ui
