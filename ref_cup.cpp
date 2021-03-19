#include "ref_cup.h"

#include <tuple> // std::make_pair

#include "ref_dice.h"
#include "ref_throw.h"
#include "ref_types.h"


namespace refac
{

Cup::Cup(Count_t dieCount, Digit_t initialDigit
  : dice(dieCount, initialDigit),
    thrown(dieCount, digitToDigitType(initialDigit)),
    active(dieCount, true)
{}

bool
Cup::operator==(Cup const& other)
{
  // The "Throw" member is consistent, comparison obsolete
  return dice == other.dice && active == other.active;
}

bool
Cup::operator==(Throw const& t)
{
  return thrown == t;
}

Count_t
Cup::anyCount() const
{
  return dice.size();
}

Count_t
Cup::activeCount(DigitType d = DigitType::total) const
{
  return thrown[d];
}

Count_t
Cup::inactiveCount() const
{
  return anyCount() - activeCount();
}

Digit_t
Cup::getDie(int pos) const
{
  assert(pos < anyCount());
  assert(pos >= 0); // TODO There is no positivity check in STL?

  return std::make_pair(dice[pos], active[pos]);
}

void
Cup::setDie(int pos, Digit_t d)
{
  auto [digit, active] = getDie(pos);
  setDie(pos, d, active);
}

void
Cup::setDie(int pos, Digit_t d, bool newActive)
{
  // Keep "Throw" part consistent by removing the old digit type (if was
  // active) and adding the new one (if will be active).
  auto [digit, oldActive] = getDie(pos);
  if (oldActive) thrown.decrement(digitToDigitType(dice[pos]));
  dice[pos] = d;
  if (newActive) thrown.increment(digitToDigitType(d        ));
}

void
Cup::addDie(Digit_t newDie, bool newActive)
{
  dice.addDie(newDie);
  thrown.increment(digitToDigitType(newDie));
  active.push_back(newActive);
}

void
Cup::addDie(DigitType newDie, bool newActive)
{
  addDie(digitTypeToDigit(newDie), newActive);
}

void
Cup::setActive(int pos, bool newActive)
{
  auto [digit, active] = getDie(pos);
  setDie(pos, digit, newActive);
}

void
Cup::toggleActive(int pos)
{
  auto [digit, active] = getDie(pos);
  setActive(pos, !active);
}

void
Cup::roll()
{
  auto it = active.cbegin();
  for (int i = 0; i < size(); ++i, ++it)
    if (*it) setDie(i, randomness::randomDie());
}

} // namespace refac
