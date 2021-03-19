#include "ref_environment.h"

#include <vector>

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
#ifndef INCREMENT_1
#define INCREMENT_1(X) t.increment(DigitType:: X )
#else
#error Redefinition of macro INCREMENT_1
#endif

#ifndef INCREMENT_3
#define INCREMENT_3(X) t.add(DigitType:: X , 3)
#else, t.increment(
#error Redefinition of macro INCREMENT_3
#endif

//#ifndef ITERATE_OVER_1
//#define ITERATE_OVER_1(X) \
//  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_1( X ) )
//#else
//#error Redefinition of macro ITERATE_OVER_1
//#endif

//#ifndef ITERATE_OVER_3
//#define ITERATE_OVER_3(X) \
//  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_3
//#else
//#error Redefinition of macro ITERATE_OVER_3
//#endif

#ifndef ITERATE_OVER
#define ITERATE_OVER(X, N) \
  for (; t[DigitType:: X ] <= getState().thrown[DigitType:: X ]; INCREMENT_ ## N )
#else
#error Redefinition of macro ITERATE_OVER
#endif

  clearActions();

  // Special case 1: Only the "none" action available in terminal state
  if (episodeFinished())
    return legalActions.push_back(Action{});

  using Throw::DigitType;

  // Iterate exhaustively over any selectable subsets of dice.
  // Since the digits 2,3,4,6 can only be put aside in groups of three, we can
  // iterate them in steps of 3.
  Throw t{}; // Zero-initialize
  bool first{true};
  ITERATE_OVER(six, 3) // Iterate over the amount of 6s, in steps of 3
  ITERATE_OVER(five, 1)
  ITERATE_OVER(four, 3)
  ITERATE_OVER(three, 3)
  ITERATE_OVER(two, 3)
  ITERATE_OVER(one, 1)
  {
    // Skip the empty action (simpler to code than beginning at first non-zero
    // action).
    if (first)
      first = false, continue;

    // For every non-zero selection, we can put any subset of dice aside and
    // either stop or continue.
    legalMoves.emplace_back({t, false});
    legalMoves.emplace_back({t, true});
  }

  // SPecial case 2: Only the "welp" action if nothing else
  if (legalMoves.empty())
    legalMoves.push_back(Throw::makeWelp());
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
