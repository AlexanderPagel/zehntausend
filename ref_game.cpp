#include "ref_game.h"

#include <algorithm>  // std::find

#include "enum.h"


namespace refac
{

bool
Game::scanLegalActions(Action const& action) const
{
  auto const& v = gameState.getEnvironment().getLegalActions();
  return std::find(v.begin(), v.end(), action) != v.end();
}

Action
Game::scanForMaxAction() const
{
  auto const& v = gameState.getEnvironment().getLegalActions();
  // Comparator to find maximal finishing action
  auto const hasLess = [](Action const& a1, Action const& a2)
  {
    return a1.taking.total() < a2.taking.total();
  };
  // Guaranteed at least 1 legal action (none or welp action)
  auto maxAct = v.crbegin();  // Reverse because I believe largest action is always last
  for (auto it = v.crbegin(); it != v.crend(); ++it)
    if (hasLess(*maxAct, *it)) maxAct = it;

  return *maxAct;
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
Game::endSubturnWith(bool finish)
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

// Find for an active die showing the desired digit and put it aside. Used to
// automatically put aside the dice for the maxAction.
void
Game::putDigitAside(DigitType d)
{
  // Search for position of active die with digit d
  auto pos = findDigit(d, true);
  toggleAside(pos);
}

// Find for an active die showing the desired digit and put it aside. Used to
// automatically put aside the dice for the maxAction.
void
Game::putDigitIn(DigitType d)
{
  // Search for position of inactive, usable die with digit d
  auto pos = findDigit(d, false);
  toggleAside(pos);
}

Count_t
Game::findDigit(DigitType d, bool a, bool u)
{
  for ( Count_t i = 0; i < getGameState().getCup().anyCount(); ++i)
    if (auto [digit, active] = getGameState().getCup().getDie(i);
        digit == digitTypeToDigit(d) && active == a && usable[i] == u)
    {
      return i;
    }

  // Unreachable: Caller must ensure search is successfull (for example by
  // checking the "Throw" part of the state when searching for a usable die).
  assert(false);
}

Game::Game()
  // Member initialization does not matter
{
  restart();
}

bool
Game::isTerminal() const
{
  return gameState.isTerminal();
}

Points_t
Game::getReturn() const
{
  assert(isTerminal()); // Otherwise points are not the return.
  return gameState.getEnvironment().getState().getPoints();
}

// Maximizes dice put aside. If necessary, puts erroneously taken out dice back
// into the cup to form a legal action.
bool
Game::toggleMax()
{
  auto const maxAction = scanForMaxAction();

  // Adjust game state to match the desired max action
  // TODO I think we way want a shorthand for this. for_DigitTypeNumbers maybe?
  for (DigitType d = DigitType::one; d <= DigitType::six; ++d)
  {
    while (maxAction.taking[d] > getGameState().getAction().taking[d])
      putDigitAside(d);
    while (maxAction.taking[d] < getGameState().getAction().taking[d])
      putDigitIn(d); // Fix erroneoulsy put aside dice
  }

  return true;
}

bool
Game::toggleAside(int pos)
{
  if (isTerminal())
    return false;

  // Internal consistency
  assert((int)usable.size() == gameState.getCup().anyCount());

  // Test for out of range position
  if (pos < 0 || gameState.getCup().anyCount()-1 < pos)
//  if (0 < pos || pos > gameState.getState().getThrown().total())
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

GameState const&
Game::getGameState() const
{
  return gameState;
}

} // namespace refac
