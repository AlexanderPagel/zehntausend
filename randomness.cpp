#include "randomness.h"

namespace randomness
{

// Only static variable to avoid static initialization yeet
decltype(std::declval<std::mt19937>()) mt{std::mt19937(std::random_device{}())};

int randomInt(int lo, int hi)
{
  return std::uniform_int_distribution<int>(lo, hi)(mt);
}

std::function<int(void)> const randomDie{std::bind(randomInt, 1, 6)};

bool epsilonRandom(double eps)
{
  return randomReal(0.0, 1.0) < eps;
}

} // namespace randomness
