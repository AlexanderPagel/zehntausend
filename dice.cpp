#include "dice.h"

namespace refac
{

void
Throw::clear(Count_t c)
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
      || (!isNone() && !other.isNone() && // not equal if only 1 == none
          finish == other.finish && throwing == other.throwing);
}

} // namespace refac
