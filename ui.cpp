#include "ui.h"

namespace ui
{

Points_t
Ui::getPoints(Player_t player) const
{
  assert(int(player) < 3);
  return game.getPoints(player);
}

}
