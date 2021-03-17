#include "dice.h"

namespace refac
{

void
Throw::clear(Count_t c)
{
  counts.fill(c);
}

Count_t Throw::operator[](DigitType d) const
{
  return counts[raw(d)];
}

Count_t& Throw::operator[](DigitType d)
{
  return counts[raw(d)];
}

Count_t Throw::total() const
{
  return counts[raw(DigitType::total)];
}

Count_t& Throw::total()
{
  return counts[raw(DigitType::total)];
}

} // namespace refac
