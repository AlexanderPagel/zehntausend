#include "display.h"

#include <array>
#include <ostream>
#include <iomanip>

namespace ui
{

namespace
{

constexpr std::array<std::string_view> constLines
{
  {"     |    Dice:    1   2   3   4   5   6             1 never changing default line"}
};

void
Display::drawPlayerLine(Ui::Game_t::Player p)
{
  os << "Player " << p << ":";
  os << std::setw(5) << ui.getPoints(p) << std::endl;
}

void
Display::drawConstantLine<L>(ConstLineIdx l)
{
  os << ConstLines.at(int(l)) << std::endl;
}

void
Ui::draw()
{

}

} // namespace ui
