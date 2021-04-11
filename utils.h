// utils.h
//
// Miscellaneous utils.


#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED 1


#include <utility>


namespace utils
{

// I just like the syntax of comma separated appending convenient
template<typename T, typename U>
auto append_more(T&& t, U&& u) -> decltype(std::forward<T>(t))
{
  // TODO forward what
  return std::forward<T>(t).append(std::forward<U>(u));
}
template<typename T, typename U, typename... Types>
auto append_more(T&& t, U&& u, Types&&... args)
{
  // TODO forward what
  return append_more(
    append_more(std::forward<T>(t), std::forward<U>(u)),
    std::forward<Types>(args)...
    );
}

} // namespace utils


#endif // UTILS_H_INCLUDED
