// enum.h
//
// Some generic enum utils. I like enums (maybe I should not).
//
// Utils for "canonical enums" := Enums that define, additionally to any other
// values:
//  - begin := the smallest legal value
//  - end   := largest legal value + 1
//  - none  := A (magic) legal value indicating that the value is not yet known

// Hard cast to underlying type, user needs to care for correctness. This is
// used explicitly to disallow all non-explicit operations between different
// enum types.


#include <cassert>
#include <type_traits>


template<typename Enum,
         typename = std::enable_if_t<std::is_enum<Enum>::value>
        >
inline constexpr auto
raw(Enum e) -> std::underlying_type_t<Enum>
{
  return static_cast<std::underlying_type_t<Enum>>(e);
}

template<typename Enum,
         typename = std::enable_if_t<std::is_enum<Enum>::value>
        >
inline constexpr auto
legit(Enum e) -> bool
{
//  using Base_t = typename std::underlying_type_t<Enum>;
  assert(  e == Enum::none
        || ((Enum::begin) <= (e) && (e) < (Enum::end)));
  return e != Enum::none;
}

// TODO
// Operation (two identical or just one) enum types:
//  - comparisons
//  - arithmetics
//  - negation
