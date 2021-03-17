#include "dice.h"

#include <cassert>

#include "enum.h"


namespace refac
{

bool
Throw::any() const
{
  assert(count[raw(DigitType::any)] >= 0);
  return counts[raw(DigitType::any)];
}

bool
Throw::empty() const
{
  return !any();
}

void
Throw::fill(Count_t c)
{
  counts.fill(c);
}

Count_t
Throw::operator[](DigitType d) const
{
  return counts[raw(d)];
}

Count_t&
Throw::operator[](DigitType d)
{
  return counts[raw(d)];
}

Count_t
Throw::total() const
{
  return counts[raw(DigitType::total)];
}

Count_t&
Throw::total()
{
  return counts[raw(DigitType::total)];
}

Action::Action()
  : throwing(), finish(true) // == "none" action
{}

bool
Action::isNone() const { return !throwing.any() && finish; }

bool
Action::operator==(Action const& other) const
{
  return (isNone() && other.isNone())
      || (!isNone() && !other.isNone() && // single "none"
          finish == other.finish && throwing == other.throwing);
}

State::State(Throw const& t, Points_t const& p)
  : thrown(t), points(p)
{}

State
State::randomStart()
{
  // TODO import the randomness sources from kniffel or pasta
}

bool
State::isTerminal()
{
  return thrown.empty() && points = 0;
}

bool
State::operator==(State const& other)
{
  return (isTerminal() && other.isTerminal())
      || (!isTerminal () && !other.isTerminal() && // single "none"
          points == other.points && thrown == other.thrown);
}

} // namespace refac
