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


// Canonical enum functions
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

template<typename T,
         typename = std::enable_if_t< std::is_same_v<T, Square>
                                    ||false>>
constexpr T
operator+(T lhs, T rhs)
{
  return static_cast<T>(raw(lhs) + raw(rhs));
}

template<typename T,
         typename = std::enable_if_t< std::is_same_v<T, Square>
                                    ||false>>
constexpr T
operator-(T lhs, T rhs)
{
  return static_cast<T>(raw(lhs) - raw(rhs));
}

template<typename T, typename U,
         typename = std::enable_if_t< std::is_same_v<T, Square>
                                    ||false>,
         typename = std::enable_if_t<std::is_integral_v<U>>
        >
constexpr T
operator-(T& lhs, U rhs)
{
  return static_cast<T>(raw(lhs) - rhs);
}

template<typename T, typename U,
         typename = std::enable_if_t< std::is_same_v<T, Square>
                                    ||false>,
         typename = std::enable_if_t<std::is_integral_v<U>>
        >
constexpr T
operator+(T& lhs, U rhs)
{
  return static_cast<T>(raw(lhs) + rhs);
}

template<typename T, typename U,
         typename = std::enable_if_t< std::is_same_v<T, Square>
                                    ||false>,
         typename = std::enable_if_t<std::is_integral_v<U>>
        >
constexpr T
operator/(T lhs, U rhs)
{
  return static_cast<T>(raw(lhs) / rhs);
}

template<typename T,
         typename = std::enable_if_t< std::is_same_v<T, Value>
                                     ||false>>
constexpr T
operator-(T const& v)
{
  return static_cast<T>(-raw(v));
}

// Commented out because I am not sure whether I will need this or not
/*
template<typename Enum,
         typename = std::enable_if_t<std::is_enum<Enum>::value>
auto
operator<(Enum const& lhs, Enum const& rhs) -> bool
{
  return raw(lhs) < raw(rhs);
}
*/
