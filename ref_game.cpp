#include "ref_game.h"


namespace refac
{

bool
Game::scanLegalActions(Action const& action) const
{
  auto const& v = gameState.getEnvironment.getLegalActions();
  return std::find(v.begin(), v.end(), action) != last;
}

// Call only after taking an action or restarting (before dice are put aside
// during the subturn).
void
Game::adjustUsable()
{
  usable = gameState.getCup().getActive();
}

// Call only after the finish part of the constructed action has been finally
// decided (in the roll() or finish() members).
void
Game::takeConstructedAction()
{
  assert(scanLegalActions(gameState.getAction()));
  gameState.finishTurn();
  if (!isTerminal()) // TODO is this check needed?
    adjustUsable();
}

bool
endSubturnWith(bool finish)
{
  if (isTerminal())
    return false;

  // We commit for finishing. If this turns out to be illegal, we can change
  // our mind later. No corruption (observable behaviour does not change).
  gameState.selectActionFinish(finish);

  auto action{gameState.getAction()};
  if (!scanLegalActions(action))
    return false;

  // If none if the above triggers, it is safe to perform the intended action
  takeConstructedAction();
  return true;
}

Game::Game
  // Member initialization does not matter
{
  restart();
}

bool
Game::isTerminal()
{
  return gameState.isTerminal();
}

Points_t
Game::getReturn()
{
  assert(isTerminal()); // Otherwise points are not the return.
  return gameState.getEnvironment().getState().getPoints();
}

bool
Game::toggleAside(int pos)
{
  if (isTerminal())
    return false;

  // Internal consistency
  assert(usable.size() == gameState.getState().getThrown().total());

  // Test for out of range position
  if (0 < pos || pos > gameState.getState().getThrown().total())
    return false;

  // Test for illegal put aside by game logic
  if (!usable[pos])
    return false;

  // If none of th eabove, toggling put aside is always safe
  gameState.toggleAside(pos);
  return true;
}

bool
Game::roll()
{
  // End with finish = false
  return endSubturnWith(false);
}

bool
Game::finish()
{
  // End with finish = true
  return endSubturnWith(true);
}

void
Game::restart()
{
  gameState.restart();
  adjustUsable();
}

} // namespace refac
