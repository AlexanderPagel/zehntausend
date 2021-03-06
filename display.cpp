#include "display.h"

#include <array>
#include <cmath>
#include <iomanip>
#include <ostream>

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

// Transform 0-based digit to character representation
// 0 -> '1', ..., 5 -> '6'
char digitToChar(int digit);

} // namespace

void
Display::clear()
{
  console::clearScreen();
}

char
Display::dieChar(int die) const
{
  return digitToChar(
      getUi()->getDieDigit(die)
      );
}

void
Display::drawPlayerLine(Game_t::Player p)
{
  os << "Player " << p << ":";
  os << std::setw(pointsWidth) << getUi()->getPoints(p);
  os << "  (" << std::setw(4) << std::fixed << std::setprecision(0)
     << std::round(getUi()->getAverage(p)) << ")";
  os << std::endl;
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
  auto const dieAppearance = [this](int i) -> std::string
  {
    if (!getUi()->getDieAvailable(i))
      return "   ";
    else if (getUi()->getDieAside(i))
      return std::string(" ") + dieChar(i) + " ";
    else
      return std::string("[") + dieChar(i) + "]";
  };

  os << "     |    " << std::setw(currentPointsWidth) << getUi()->getTurnPoints();
  os << (getUi()->canStopTurn() ? "   " : " ->");
  for (int i = 0; i < 6; ++i)
  {
    os << " ";
    os << dieAppearance(i);
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

namespace
{

char digitToChar(int digit)
{
  // If digit definition ever changes we can change assert. For now
  // we expect digits in range [1, 6].
  assert(1 <= digit && digit <= 6);
  return '0' + digit;
}

} // namespace

} // namespace ui
