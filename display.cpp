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
  {"     |    Dice:    1   2   3   4   5   6             1 never changing default line"}
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
  os << std::setw(pointsWidth) << ui.getPoints(p) << std::endl;
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
  os << "   " << ui.getPlayer() << " |" << std::endl;
}

void
Display::drawCurrentThrowLine()
{
  os << "     |    " << std::setw(currentPointsWidth) << ui.getTurnPoints();
  os << (ui.canStopTurn() ? "   " : " ->");
  for (int i = 0; i < 6; ++i)
  {
    if (ui.getDieAside(i)) os << "  " << ui.getDieDigit(i) << " ";
    else                   os << " [" << ui.getDieDigit(i) << "]";
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

Display::Display(Ui const& ui)
  : ui(ui)
{}

} // namespace ui
