#include "display.h"

#include <ostream>
#include <iomanip>

namespace ui
{

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
