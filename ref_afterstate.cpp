#include "ref_afterstate.h"

#include "ref_environment.h" // Point computation


namespace refac
{

Afterstate
Afterstate::createTerminalTransition()
{

  // These do not matter
  Afterstate as;
  as.pointsBefore = 0;
  as.pointsAfter = 0;
  as.x.diceLeft = 0;
  as.x.welping = false;
  as.x.intoTerminal = true; // Does not matter, still setting it :)

  // This indicates a terminal transition, ignoring everything else
  as.x.fromTerminal = true;

  return as;
}

Afterstate
Afterstate::createFromTransition(State const& state, Action const& action)
{
  // Special case: Terminal states
  if (state.isTerminal())
    return createTerminalTransition();

  Afterstate as;
  as.pointsBefore = state.getPoints();
  as.x.fromTerminal = false;

  // Since we assume legal actions, the action shows whether
  // a terminal state is reached.
  if (action.finish)
  {
    as.x.diceLeft = 0; // Arbitrary dummy value
    as.x.intoTerminal = true;
    if (action.isWelp())
    {
      // Case 1: Finish by welping (= lose all points)
      as.pointsAfter = 0;
      as.x.welping = true;
    }
    else
    {
      // Case 2: Finish by decision (= secure all points)
      as.pointsAfter = Environment::pointsWorthRaw(action);
      as.x.welping = false;
    }
  }
  else
  {
    // Case 3: Continue (= risk all points)
    auto const countAdjust = [&](Count_t c)
    {
      auto tmp{c - action.taking.total()};
      return tmp == 0 ? totalGameDieCount : tmp;
    };
    // TODO Here is potential speedup by storing the associated worth
    //      within each action.
    as.pointsAfter = Environment::pointsWorthRaw(action);
    as.x.diceLeft = countAdjust(state.getThrown().total());
    as.x.intoTerminal = false;
    as.x.welping = false;
  }

  return as;
}

Count_t
Afterstate::diceLeft() const
{
  return x.diceLeft;
}

bool
Afterstate::intoTerminal() const
{
  return x.intoTerminal;
}

bool
Afterstate::fromTerminal() const
{
  return x.fromTerminal;
}

bool
Afterstate::fromWelp() const
{
  return x.welping;
}

Points_t
Afterstate::pointGain() const
{
  assert(!fromTerminal());
  return pointsAfter - pointsBefore;
}

bool
Afterstate::operator==(Afterstate const& other) const
{
  // Special case: Acting in a terminal state
  if (fromTerminal() || other.fromTerminal())
    return fromTerminal() == other.fromTerminal();

  // When afterstates finish, the dice left do not matter
  if (intoTerminal() || other.intoTerminal())
  {
    return intoTerminal() == other.intoTerminal()
        // When finishing, points must not match exactly.
        && pointGain() == other.pointGain();
  }
  else
  {
    return diceLeft() == other.diceLeft()
        // When continuing the episode,
        && pointsBefore == other.pointsBefore
        && pointsAfter == other.pointsAfter;
  }
}

Afterstate::Afterstate(State const& state, Action const& action)
  : Afterstate(createFromTransition(state, action))
{}

} // namespace refac
