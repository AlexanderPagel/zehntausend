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
Environment::fillActions()
{
  clearActions();

  // Special case 1: Only the "none" action available in terminal state
  if (episodeFinished())
    return legalActions.push_back(Action{});

  // Iterate exhaustively over any selectable subsets of dice.
  // Since the digits 2,3,4,6 can only be put aside in groups of three, we can
  // iterate them in steps of 3.
  Throw t{}; // Zero-initialize
  bool first{true};
  // TODO Remove unnecessary tracking of the total count.
  for (; t.getDigitCount(DigitType::six) <= getState().getThrown().getDigitCount(DigitType::six); t.add(DigitType::six, 3))
  for (; t.getDigitCount(DigitType::five) <= getState().getThrown().getDigitCount(DigitType::five); t.increment(DigitType::five))
  for (; t.getDigitCount(DigitType::four) <= getState().getThrown().getDigitCount(DigitType::four); t.add(DigitType::four, 3))
  for (; t.getDigitCount(DigitType::three) <= getState().getThrown().getDigitCount(DigitType::three); t.add(DigitType::three, 3))
  for (; t.getDigitCount(DigitType::two) <= getState().getThrown().getDigitCount(DigitType::two); t.add(DigitType::two, 3))
  for (; t.getDigitCount(DigitType::one) <= getState().getThrown().getDigitCount(DigitType::one); t.increment(DigitType::one))
  {
    // Skip the empty action (simpler to code than beginning at first non-zero
    // action).
    if (first)
    {
      first = false;
      continue;
    }

    // For every non-zero selection, we can put any subset of dice aside and
    // either stop or continue.
    legalActions.emplace_back(t, false);
    legalActions.emplace_back(t, true);
  }

  // SPecial case 2: Only the "welp" action if nothing else
  if (legalActions.empty())
    legalActions.push_back(Action::makeWelp());
}

Environment::Environment()
  : state(State::startState()),
    legalActions{} // Initialize empty
{
  fillActions();
}

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
    state = State::makeTerminalState();
    goto adjust_points;
  }

  // Compute immediate point diff for given action
  for (auto d = DigitType::one;
       d != DigitType::six; ++d)
  {
    // Every triple if 4s scores 400 etc.
    pointDiff += action.taking[d] / 3 * 100 * digitTypeToDigit(d);
  }
  // All 1s and 5s of non-triples score 100 (resp. 50) each
  pointDiff += (action.taking[DigitType::one ] - action.taking[DigitType::one ]/3*3) * 100;
  pointDiff += (action.taking[DigitType::five] - action.taking[DigitType::five]/3*3) * 50 ;

  // Compute new thrown dice
  if (action.finish)
  {
    // Can ignore rolled dice if finishing
    state = State::makeTerminalState();
  }
  else
  {
    // Need to re-roll leftovers to continue
    state -= action.taking;
    state.roll(totalGameDieCount);
  }

adjust_points:
  // Accumulate total return G
  state.addPoints(pointDiff);
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
  return legalActions;
}

void
Environment::restart()
{
  state = State::startState();
  fillActions();
}

} // namespace refac
