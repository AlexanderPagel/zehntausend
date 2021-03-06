#include "ref_game_state.h"

#include <cassert>
#include <tuple> // std::get

#include "enum.h"
#include "randomness.h"


namespace refac
{

// Only call this member at the beginning of each subturn (when no dice have
// been put aside yet), so that the total number of dice in 'thrown' match the
// number of active dice in 'dice'.
void
GameState::makeDigitsConsistent()
{
  // Otherwise can not possibly be consistent
  assert(cup.activeCount() == getState().getThrown().total());

  // Only run before constructing action
  assert(action.taking.empty());

  // Get indices of all active dice
  auto const& freeBit = cup.getActive();
  auto freePos = indicesOf(freeBit.begin(), freeBit.end(),
                           [](bool b){ return b; });

  // Internal object 'getState()' dictates how many of each DigitType there are
  for (DigitType d = DigitType::one; d <= DigitType::six; ++d)
    for (auto n = getState().getThrown()[d]; n; --n)
    {
      // Object 'cup' is set to a random permutation of dictated digits. Only
      // active positions are considered.
      auto posIt = randomness::pickUniformRandom(freePos);
      cup.setDie(*posIt, digitTypeToDigit(d));

      // Validate correctness of index filter. Remove assert later.
      assert(std::get<bool>(cup.getDie(*posIt)));

      freePos.erase(posIt);
    }

  // Validate algorithm correctness. Remove assert later.
  assert(freePos.empty());
}

void
GameState::cupResize()
{
  // This assert can be removed if the game rule change in a way
  // breaking this rule.
  assert(cup.activeCount() == 0); // Only after roll
  cup = Cup(getState().getThrown().total());
}

GameState::GameState()
  : environment(), // Dictate initial state
    cup(getState().getThrown().total()) // Initialize to correct number of dice
{
  // Make member 'cup' consistent to the dictated initial state
  makeDigitsConsistent();

  // TODO Maybe just restart()? Performance does not matter anyway
}

Environment const&
GameState::getEnvironment() const
{
  return environment;
}

Cup const&
GameState::getCup() const
{
  return cup;
}

State const&
GameState::getState() const
{
  return getEnvironment().getState();
}

Action const&
GameState::getAction() const
{
  return action;
}

bool
GameState::isTerminal() const
{
  return getState().isTerminal();
}

void
GameState::toggleAside(int pos)
{
  assert(!isTerminal());

  auto [digit, aside] = cup.getDie(pos);
  if (aside) action.taking.increment(digitToDigitType(digit));
  else       action.taking.decrement(digitToDigitType(digit));
  cup.toggleActive(pos);
}

// Sets the finish member of the internally constructed action, for
// a subsequent call to finishTurn() without argument.
void
GameState::selectActionFinish(bool finishes)
{
  action.finish = finishes;
}

// Requires that selectActionFinish() was called beforehand (by the user, when
// called directly, or by the finishTurn(bool) member.
void
GameState::finishTurn()
{
  // Use 'state' to dictate next state. Keeping 'cup' consistent.
  // Taking the constructed action in the environment makes
  environment.takeAction(action);
  action = Action::makeWelp();
  // TODO Alternatively check if action has/would re-roll all dice. I did not
  //      like that alternative as much because it relies on game mechanics.
  // If taking the constructed action changes the number of dice left we simply reset the cup. (This would scramble the non-changing dice if the
  if (!isTerminal())
  {
    if (getState().getThrown().total() != cup.activeCount())
    {
      // Only option following game rules is that all dice were re-activated by
      // action. Remove this assert if the game rules change to allow different
      // behaviour.
      assert(getState().getThrown().total() == cup.anyCount());
      cupResize();
    }
    makeDigitsConsistent();
  }

  // If terminal state is reached, environment will not (and doesn't need to)
  // provide a throw again and numbers can legitimately mismatch.
  assert(isTerminal() ||
         getState().getThrown().total() == getCup().activeCount());
}

void
GameState::finishTurn(bool finish)
{
  assert(!isTerminal());

  // Finalize action
  selectActionFinish(finish);

  // Use constructed action
  finishTurn();
}

void
GameState::restart()
{
  environment.restart();
//  cupResize();
  cup = Cup(getState().getThrown().total()); // Create correct umber of dice
  makeDigitsConsistent();
  action = Action::makeWelp();
}

} // namespace refac
