#include "ui.h"

namespace ui
{

Game_t::Player
Ui::getPlayer() const
{
  // TODO assert(not terminal) once gmae class has such a state
  return game.getPlayer();
}

Points_t
Ui::getTurnPoints() const
{
  return game.getCurrent();
}

bool
Ui::getDieAside(int) const
{
  // TODO Not implemented in game class or need to go over state
  assert(false);
}

int
Ui::getDieDigit(int idx) const
{
  assert(0 <= idx && idx <= 6);

  return game._cup[idx];
}

bool
Ui::canStopTurn() const
{
  return game.getPutAny();
}

State_t
Ui::getState() const
{
  return game.state();
}

Points_t
Ui::getPoints(Player_t player) const
{
  assert(int(player) < 3);
  return game.getPoints(player);
}

}
