#include "ref_dice.h"

#include <algorithm> // std::transform
#include <cassert>

#include "randomness.h"
#include "ref_types.h"


namespace refac
{

Dice::Dice(Count_t c, Digit_t d)
  : dice(c, d) // Number of elements = c, initial value = d
{}

Digit_t
Dice::operator[](int pos) const
{
  assert(pos < (int) dice.size());

  return dice[pos];
}

Digit_t&
Dice::operator[](int pos)
{
  assert(pos < (int) dice.size());

  return dice[pos];
}

Count_t
Dice::count() const
{
  assert(dice.size() < (unsigned long long) std::numeric_limits<Count_t>::max());
  return static_cast<Count_t>(dice.size());
}

bool
Dice::operator==(Dice const& other) const
{
  return dice == other.dice;
}

void
Dice::roll()
{
  std::transform(
      dice.begin(), dice.end(), dice.begin(),
      [](auto&) { return randomness::randomDie(); }
      );
}

void
Dice::roll(Selection_t const& selection)
{
  assert(selection.size() == dice.size());

  auto selIt = selection.begin();
  for (auto it = dice.begin(); it != dice.end(); ++it, ++selIt)
    if (*selIt) *it = randomness::randomDie();
}

void
Dice::addDie(Digit_t newDie)
{
  dice.push_back(newDie);
}

} // namespace refac
