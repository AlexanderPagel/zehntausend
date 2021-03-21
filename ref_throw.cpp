#include "ref_throw.h"

#include <cassert>

#include "randomness.h"
#include "ref_types.h"
#include "enum.h"


namespace refac
{

Count_t&
Throw::operator[](DigitType d)
{
  return counts[raw(d)];
}

Count_t&
Throw::at(DigitType d)
{
  assert(legit(d));
  assert(raw(d) >= 0 && raw(d) < (int)counts.size());

  return (*this)[d];
}


Count_t&
Throw::total()
{
  return counts[raw(DigitType::total)];
}

Throw::Throw()
    // Init to DT::count copies of the value 0
  : counts(raw(DigitType::count), Count_t(0))
{}

Throw::Throw(Count_t dieCount, DigitType initialDigit)
  : Throw() // Initialize with zero dice
{
  add(initialDigit, dieCount);
}

bool
Throw::any() const
{
  assert(counts[raw(DigitType::total)] >= 0);
  return counts[raw(DigitType::total)];
}

bool
Throw::empty() const
{
  return !any();
}

bool
Throw::operator==(Throw const& other) const
{
  // TODO Compare totals first, then digits
  return counts == other.counts;
}

Count_t
Throw::getDigitCount(DigitType d) const
{
  assert(legit(d));

  return counts[raw(d)];
}

bool
Throw::consistent() const
{
  Count_t sum{0};
  for (auto d = DigitType::one;
      d <= DigitType::six;
      ++d)
  {
    auto c = getDigitCount(d);
    if (c < 0) return false; // Ensure non-negative
    sum += c;
  }
  return sum == total(); // Ensure total is consistent
}

void
Throw::fill(Count_t c)
{
  auto constexpr first = raw(DigitType::one); // 0
  auto constexpr last = raw(DigitType::six);  // 5
  std::fill(counts.begin() + first,
            counts.begin() + last + 1,
            c);
  // Keep total cinsistent
  counts[raw(DigitType::total)] = c * (last - first);
}

void
Throw::setDigitCount(DigitType d, Count_t newValue)
{
  assert(legit(d));
  assert(0 >= newValue);

  // TODO Can use member access to counts array throughout the "Throw" member
  // implementations.

  auto diff = newValue - getDigitCount(d);
  counts[raw(d               )]  = newValue;
  counts[raw(DigitType::total)] += diff;
}

void
Throw::add(DigitType d, Count_t c)
{
  assert(getDigitCount(d) + c >= 0);

  counts[raw(d)] += c;
  counts[raw(DigitType::total)] += c;
}

void
Throw::remove(DigitType d, Count_t c)
{
  add(d, -c);
}

void
Throw::increment(DigitType d)
{
  ++counts[raw(d               )];
  ++counts[raw(DigitType::total)];
}

void
Throw::decrement(DigitType d)
{
  assert(0 < counts[raw(d)]);

  --counts[raw(d               )];
  --counts[raw(DigitType::total)];
}

void
Throw::roll()
{
  roll(total());
}

void
Throw::roll(Count_t c)
{
  assert(c >= 0);

  fill(0);
  for (int i = 0; i < c; ++i)
  {
    auto randomdie{randomness::randomDie()}; // [1, 6]
    // TODO 6 separate increments to total are not necessary
    increment(
        // Convert 1-based to 0-based
        digitToDigitType(randomdie)
        );
  }
}

Throw&
Throw::operator-=(Throw const& other)
{
  // TODO I could not find an STL algorithm that does in-place transformations
  for (DigitType d = DigitType::begin;
       d != DigitType::end; // 1-6 and total
       ++d)
  {
    counts[raw(d)] -= other.counts[raw(d)];
  }

  assert(consistent());
  return *this;
}

Count_t
Throw::operator[](DigitType d) const
{
  return counts[raw(d)];
}

Count_t
Throw::total() const
{
  return counts[raw(DigitType::total)];
}

} // namespace refac
