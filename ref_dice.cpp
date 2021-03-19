#include "ref_dice.h"

#include <algorithm> // std::transform

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

void
Dice::roll()
{
  std::transform(
      dice.begin(), dice.end(), dice.begin(),
      [](auto d) { d = randomness::randomDie(); }
      );
}

void
Dice::roll(Selection const& selection)
{
  assert(selection.size() == dice.size());

  auto selIt = selection.begin();
  for (auto it = dice.begin(); it != dice.end(); ++it, ++selIt)
    if (*selit) *it = randomness::randomDie();
}

void
Dice::addDie(Digit_t newDie)
{
  dice.push_back(newDie);
}

} // namespace refac
