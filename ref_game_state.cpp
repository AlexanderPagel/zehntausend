#include "ref_game_state.h"

#include <cassert>
#include <tuple> // std::get


namespace refac
{

void
GameState::makeDigitsConsistent()
{
  // Otherwise can not possibly be consistent
  assert(cup.activeCount() == getState().getThrow().total());

  // Only ran before constructing action
  assert(action.taking.empty());

  // Get indices of all active dice
  auto const& freeBit = cup.getActive();
  auto freePos = indicesOf(freeBit.begin(), freeBit.end(),
                           [](bool b){return b});

  // Internal object 'getState()' dictates how many of each DigitType there are
  for (DigitType d = DigitType::one; d <= DigitType::six; ++d)
    for (auto n = getState().getThrow()[d]; n; --n)
    {
      // Object 'cup' is set to a random permutation of dictated digits. Only
      // active positions are considered.
      auto posIt = pickUniformRandom(freePos);
      cup.setDie(*posIt, d);

      // Validate correctness of indice filter. Remove assert later.
      assert(std::get<bool>(getDie(*posIt)));

      freePos.erase(posIt);
    }

  // Validate algorithm correctness. Remove assert later.
  assert(freePos.empty());
}

GameState::GameState()
  : environment(), // Dictate initial state
    cup(getState().getThrow().total()) // Initialize to correct number of dice
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

bool
GameState::isTerminal() const
{
  return getState().isTerminal();
}

void
GameState::toggleAside(pos)
{
  assert(!isTerminal());

  auto [digit, aside] = cup.getDie(pos);
  if (aside) action.taking.decrement[digitToDigitType(digit)];
  else       action.taking.increment[digitToDigitType(digit)];
  cup.toggleActive(pos);
}

bool
GameState::selectActionFinish(bool finishes)
{
  action.finish = finishes;
}

void
GameState::finishTurn(bool finish)
{
  assert(!isTerminal());

  // Finalize action
  selectActionFinish(finish);

  // Use 'state' to dictate next state. Keeping 'cup' consistent.
  // Taking the constructed action in the environment makes
  environment.takeAction(action);
  makeDigitsConsistent();
  action = Action::makeWelp();

  // If terminal state is reached, environment will not (and doesn't need to)
  // provide a throw again and numbers can legitimately mismatch.
  assert(isTerminal() ||
         getState().getThrown().total() == getCup().activeCount());
}

void
GameState::restart()
{
  // TODO Remove assert later. For now lets just make sure we don't quit too
  //      early:
  assert(isTerminal());

  environment.restart();
  cup = Cup(getState().getThrown().total()); // Create correct umber of dice
  makeDigitsConsistent();
  action = Action::makeWelp();
}

} // namespace refac
