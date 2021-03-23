#include "ref_state.h"

#include <utility> // std::move

#include "ref_throw.h"
#include "ref_types.h"


namespace refac
{

State::State(Throw const& t, Points_t const& p)
  : thrown(t), points(p)
{}

State
State::startState()
{
  Throw initialThrow;
  initialThrow.roll(totalGameDieCount);
  return State(std::move(initialThrow), Points_t(0));
}

State
State::makeTerminalState()
{
  return State();
}

bool
State::isTerminal() const
{
  return terminal;
}

bool
State::operator==(State const& other) const
{
  return (isTerminal() && other.isTerminal())
      || (!isTerminal () && !other.isTerminal() && // single "none"
          points == other.points && thrown == other.thrown);
}

Points_t
State::getPoints() const
{
  return points;
}

Throw const&
State::getThrown() const
{
  return thrown;
}

void
State::addPoints(Points_t p)
{
  points += p;
}

void
State::setTerminal(bool t)
{
  terminal = t;
}

void
State::roll(Count_t n)
{
  if (thrown.empty())
    thrown.roll(n); // Randomizes n new dice
  else
    thrown.roll();  // Randomizes existing dice
}

void
State::operator-=(Throw const& t)
{
  thrown -= t;
}

} // namespace refac
