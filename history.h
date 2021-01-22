// history.h
//
// Class History
//
// The class History represents a state history starting at the very start of the game.
// Repräsentiert eine state history.

#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED 1


#include <vector>

#include "ui_subobject.h"


namespace ui
{

template<typename S>
class History : public UiSubobject, private std::vector<S>
{
  public:
    using State_t = S;
    using Vec_t = std::vector<S>;

    // Sets UI object, vector is defaulted
    using UiSubobject::UiSubobject;
};

} // namespace ui


#endif // HISTORY_H_INCLUDED
