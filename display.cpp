#include "display.h"

#include <array>
#include <ostream>
#include <iomanip>

#include "console.h"
#include "ui.h"

namespace ui
{

namespace
{

constexpr std::string_view constLines[] =
{
  // Indices must match Display::ConstlineIdx
  {""},
  {"     |    Dice:    1   2   3   4   5   6"}
};

} // namespace

void
Display::clear()
{
  console::clearScreen();
}

void
Display::drawPlayerLine(Game_t::Player p)
{
  os << "Player " << p << ":";
  os << std::setw(pointsWidth) << getUi()->getPoints(p) << std::endl;
}

void
Display::drawConstantLine(ConstLineIdx l)
{
  assert(unsigned(l) < std::size(constLines));
  os << constLines[int(l)] << std::endl;
}

void
Display::drawPlayerToMoveLine()
{
  os << "   " << getUi()->getPlayer() << " |" << std::endl;
}

void
Display::drawCurrentThrowLine()
{
  os << "     |    " << std::setw(currentPointsWidth) << getUi()->getTurnPoints();
  os << (getUi()->canStopTurn() ? "   " : " ->");
  for (int i = 0; i < 6; ++i)
  {
    if (getUi()->getDieAside(i)) os << "  " << getUi()->getDieDigit(i) << " ";
    else                         os << " [" << getUi()->getDieDigit(i) << "]";
  }
  os << std::endl;
}

void
Display::draw()
{
  clear();
  for (Player_t p = 0; p < 3; ++p)
    drawPlayerLine(p);
  drawConstantLine(ConstLineIdx::emptyLine);
  drawConstantLine(ConstLineIdx::diceNumLine);
  drawPlayerToMoveLine();
  drawCurrentThrowLine();
}

} // namespace ui
