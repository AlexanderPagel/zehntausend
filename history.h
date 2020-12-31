// history.h
//
// Class History
//
// The class History represents a state history starting at the very start of the game.
// Repräsentiert eine state history.

#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED 1

#include <vector>

namespace ui
{

template<typename S>
class History : private std::vector<S>
{
  public:
    using State_t = S;
    using Vec_t = std::vector<S>;
};

} // namespace ui


#endif // HISTORY_H_INCLUDED
