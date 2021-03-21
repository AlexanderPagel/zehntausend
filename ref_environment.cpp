#include "ref_environment.h"

#include <vector>

#include "enum.h"
#include "ref_action.h"
#include "ref_state.h"
#include "ref_types.h"


namespace refac
{

void
Environment::clearActions()
{
  legalActions.clear();
}

void
Environment::fillActions() const
{
  legalActions = generateActions(getState());
}

std::vector<Action>
Environment::generateActions(State const& state)
{
  // TODO Optimization potential: Points computation on-the fly while
  //      generating action.

  std::vector<Action> legalActions{};

  // Special case 1: Only the "none" action available in terminal state
  if (state.isTerminal())
  {
    legalActions.push_back(Action::makeNone());
    return legalActions;
  }

  // Iterate exhaustively over any selectable subsets of dice.
  // Since the digits 2,3,4,6 can only be put aside in groups of three, we can
  // iterate them in steps of 3.
  Throw t{}; // Zero-initialize
  bool first{true};
  for (t.setDigitCount(DigitType::six, 0); t.getDigitCount(DigitType::six) <= state.getThrown().getDigitCount(DigitType::six); t.add(DigitType::six, 3))
  for (t.setDigitCount(DigitType::five, 0); t.getDigitCount(DigitType::five) <= state.getThrown().getDigitCount(DigitType::five); t.increment(DigitType::five))
  for (t.setDigitCount(DigitType::four, 0); t.getDigitCount(DigitType::four) <= state.getThrown().getDigitCount(DigitType::four); t.add(DigitType::four, 3))
  for (t.setDigitCount(DigitType::three, 0); t.getDigitCount(DigitType::three) <= state.getThrown().getDigitCount(DigitType::three); t.add(DigitType::three, 3))
  for (t.setDigitCount(DigitType::two, 0); t.getDigitCount(DigitType::two) <= state.getThrown().getDigitCount(DigitType::two); t.add(DigitType::two, 3))
  for (t.setDigitCount(DigitType::one, 0); t.getDigitCount(DigitType::one) <= state.getThrown().getDigitCount(DigitType::one); t.increment(DigitType::one))
  {
    // Skip the empty action (simpler to code than beginning at first non-zero
    // action).
    if (first)
    {
      first = false;
      continue;
    }

    // For every non-zero selection, we can put any subset of dice aside and
    // continue. If we gain enough points, we can also finish.
    legalActions.emplace_back(t, false);
    if (pointsWorthLimit(t, state.getPoints()) > 0)
      legalActions.emplace_back(t, true);
  }

  // Special case 2: Only the "welp" action if nothing else
  if (legalActions.empty())
    legalActions.push_back(Action::makeWelp());

  return legalActions;
}

Points_t
Environment::pointsWorthRaw(Throw const& thrown)
{
  Points_t points{0};

  auto const tripleWorth = [](Count_t c, DigitType d, Points_t base = 100) -> Points_t
  {
    return c / 3 * base * digitTypeToDigit(d);
  };
  auto const singleWorth = [](Count_t c, Points_t base) -> Points_t
  {
    return (c - c/3*3) * base;
  };

  // Every triple of 1 scores 1000.
  // Every triple of {2,3,4,5,6} scores 100x the digit value.
  points += tripleWorth(thrown[DigitType::one], DigitType::one, 1000);
  for (auto d = DigitType::two; d <= DigitType::six; ++d)
    points += tripleWorth(thrown[d], d);

  // All 1s and 5s of non-triples score 100 (resp. 50) each
  points += singleWorth(thrown[DigitType::one], 100);
  points += singleWorth(thrown[DigitType::five], 50);

  return points;
}

Points_t
Environment::pointsWorthLimit(Throw const& thrown, Points_t startPoints)
{
  // TODO Worth to test if startpoints > turn limit beforehand?

  if (auto sum{pointsWorthRaw(thrown) + startPoints}; sum >= turnLimit)
    return sum;
  else
    return Points_t{0};
}

Points_t
Environment::pointsWorthRaw(Action const& action)
{
  return pointsWorthRaw(action.taking);
}

Points_t
Environment::pointsWorthLimit(Action const& action, Points_t startPoints)
{
  return pointsWorthLimit(action.taking, startPoints);
}

Environment::Environment()
  : state(State::startState()),
    legalActions{} // Initialize empty
{}

// Action must be known-legal
Points_t
Environment::takeAction(Action const& action)
{
  Points_t pointDiff{0};

  // Special case 1: Terminal states have trivial behaviour
  if (episodeFinished())
  {
    assert(action.isNone());
    pointDiff = 0;
    goto adjust_points;
  }
  assert(!action.isNone());

  // Special case 2: The welp action leads to terminal state, loosing all points
  if (action.isWelp())
  {
    // All current points are lost by welping
    pointDiff = -state.getPoints();
    state.setTerminal();
    goto adjust_points;
  }

  // Compute immediate point diff for given action
  pointDiff = pointsWorthRaw(action);

  // Compute new thrown dice
  if (action.finish)
  {
    // Can ignore leftover dice if finishing but...
    state.setTerminal();
  }
  else
  {
    // ...need to re-roll leftovers to continue.
    state -= action.taking;
    state.roll(totalGameDieCount);  // Rolls new dice if exactly 0 left
  }

adjust_points:
  // Accumulate total return G
  // TODO Alternatively work on state and return different to points
  //      at beginning of this function.
  state.addPoints(pointDiff);

  clearActions();

  // Return immediate reward R
  return pointDiff;
}

bool
Environment::episodeFinished() const
{
  return getState().isTerminal();
}

State const&
Environment::getState() const
{
  return state;
}

std::vector<Action> const&
Environment::getLegalActions() const
{
  if (legalActions.empty())
    fillActions();
  return legalActions;
}

void
Environment::restart()
{
  state = State::startState();
  fillActions();
}

} // namespace refac
