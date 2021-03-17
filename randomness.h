// randomness.h
//
// Global randomness generation

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED 1

#include <functional>
#include <random>
#include <utility>

namespace randomness
{

using namespace std::placeholders;

// Global source of randomness
extern decltype(std::declval<std::mt19937>()) mt;

extern int randomInt(int lo, int hi);
template<typename T>
extern T randomReal(T lo, T hi);

extern std::function<int(void)> const randomDie;

//template<typename D>
//extern auto useGlobalRandomness(D& d) -> std::function<decltype(d(mt)) ()>;

} // namespace randomness

#include "randomness.inc"

#endif // RANDOM_H_INCLUDED
