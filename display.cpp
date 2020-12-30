#include "display.h"

#include <ostream>
#include <iomanip>

namespace ui
{

// Preliminary screen setup:

/*
Player 1:    50                                                                  3 player lines
Player 2:   300                                                                  .
Player 3: 10000                                                                  .
                                                                                 1 empty line
     |    Dice:    1   2   3   4   5   6                                         1 never changing default line
   0 |                                                                           1 line just with player number
     |      0  -> [3] [3] [3] [6] [1] [5]                                        1 line with rolled dice,
                                                                                             currernt points,
                                                                                             active indivcators, and
                                                                                             yet-to-move indicators
*/

void
Ui::drawPlayer(Ui::Game_t::Player p)
{
  drawPlayerLine(os,
                 p,
                 ui.getPoints(p));
}

void
Ui::draw()
{

}

void
drawPlayerLine(
    std::ostream& os,
    Ui::Game_t::Player p,
    int points)
{
  os << "Player " << p << ":";
  os << std::setw(5) << points << std::endl;
}


} // namespace ui
