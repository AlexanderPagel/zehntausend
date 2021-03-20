// ref_types.h
//
// Provide basic types underlying the Game and Environment simulation.

#ifndef REF_TYPES_H_INCLUDED
#define REF_TYPES_H_INCLUDED 1


#include <cstdint>
#include <vector>


namespace refac
{

// Represent the "number" a die is showing, literally
using Digit_t = int_least8_t; // In range [1, 6].
// Represent the number of present game objects, like dice
using Count_t = int_least8_t; // Allow small arrays by using smallest type.

using Player_t = int_fast8_t; // TODO i think i just used int here, replace int in code?
using Selection_t = std::vector<bool>;
using Points_t = int_fast32_t;
// TODO typedef Active_t = bool?

// TODO do we want to move this somewhere else? into a class?
int constexpr totalGameDieCount{6};

// While Digit_t is 1-based and the value represents a digit, DigitType is
// 0-based (to index arrays easily). Use typed enum to avoid mixing the two
// accidentally.
enum class DigitType : Digit_t
{
  one, two, three, four, five, six, total, // 0..6, 7, used to index arrays
  count, begin = 0, end = count, none = -1 // canonical enum members
};

// Convert 1-based, valid digit to corresponding digit type
DigitType digitToDigitType(Digit_t   digit);
Digit_t   digitTypeToDigit(DigitType digit);


} // namespace refac


// TODO Put these into some utils source or anything?
namespace refac
{

template<typename Input>
bool truePredicate(Input)
{
  return true;
}
template<typename It, typename Pred>
auto indicesOf(It begin, It end, Pred p = truePredicate<decltype(*std::declval<decltype(begin)>())>)
  -> std::vector<decltype(end - begin)>
{
  std::vector<decltype(end - begin)> res{};
  for (auto it = begin; it != end; ++it)
  {
    // Add indices of elements that fulfill p
    if (p(*it)) res.push_back(it - begin);
  }
  return res;
}

} // namespace refac


#endif // REF_TYPES_H_INCLUDED
