#include "dice.h"

#include <cassert>

#include "enum.h"
#include "randomness.h"


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
  assert(d >= 0 && d < counts.size());

  return (*this)[d];
}


Count_t&
Throw::total()
{
  return counts[raw(DigitType::total)];
}

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

Throw::Count_t
Throw::getDigitCount(DigitType d) const
{
  assert(legit(d));

  return counts[raw(d)];
}

bool
Throw::consistent() const
{
  Counts_t sum{0};
  for (auto d = DigitType::one;
      d <= DigitType::six;
      ++d)
  {
    if (at(d) < 0) return false; // Ensure non-negative
    sum += *it;
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
  assert(counts[raw(d)] > 0);

  --counts[raw(d               )];
  --counts[raw(DigitType::total)];
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

Throw::DigitType
Throw::digitToDigitType(int digit)
{
  // Add offset int -> DigitType
  auto constexpr offset = raw(DigitType::one) - 1
  auto const res = DigitType(digit + offset);

  assert(legit(res));

  return res;
}

Action::Action()
  : throwing(), finish(true) // == "none" action
{}

bool
Action::isNone() const { return throwing.empty() && !finish; }

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
State::startState()
{
  Throw initialThrow;
  for (int i = 0; i < totalGameDieCount; ++i)
  {
    auto randomDie{randomness::randomDie()};
    // TODO 6 separate increments to total are not necessary
    initialThrow.increment(
        // Convert 1-based to 0-based
        Throw::digitToDigitType(randomDie)
        );
  }

  return State(std::move(initialThrow), Points_t(0));
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

void
Environment::fillLegalActions()
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

  // Two special cases:
  //  - State is terminal -> only the "none" action is valid
  //  - Non-terminal state but no dice can be selected -> only the "welp" action is valid

  using Throw::DigitType;

  // Iterate exhaustively over any selectable subsets of dice.
  // Since the digits 2,3,4,6 can only be put aside in groups of three, we can
  // iterate them in steps of 3.
  Throw t{}; // Zero-initialize
  bool first{true};
  ITERATE_OVER(six, 3)
  ITERATE_OVER(five, 1)
  ITERATE_OVER(four, 3)
  ITERATE_OVER(three, 3)
  ITERATE_OVER(two, 3)
  ITERATE_OVER(one, 1)
  {
    // Skip the empty action (simpler to code than beginning at first non-zero
    // action).
    if (first) first = false, continue;

    // For every non-zero selection, we can put any subset of dice aside and
    // either stop or continue.
    legalMoves.emplace_back({t, false});
    legalMoves.emplace_back({t, true});
  }
}

} // namespace refac
