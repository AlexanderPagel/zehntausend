// ui_rollback.h
//
// Class ui::Rollback implementing a rame rollback.
//
// Allows to at any point create a checkpoints of the current game state. Old
// checkpoints are overwritten by the most recent checkpoint.
//
// The intended use is to create a checkpoint after each "irreversibe" action,
// such as rolling or finishing the turn. If during "reversible" actions
// (putting dice aside), a human typo occurs, the (only reversible) actions
// since the last checkpoint (the last irreversible) can be revised.


#ifndef ROLLBACK_H_INCLUDED
#define ROLLBACK_H_INCLUDED 1


#include "ui_subobject.h"
#include "ui_types.h"


namespace ui
{

// Typename T is the Game type. Typically Tenthousand<1> or Tenthousand<3>.
// We use this to simply store a game as a subobject to take snapshots of
// a game state. If we want to restore the game state, we simply continue in
// the previous snapshot.
template<typename T = ui::Game_t>
class Rollback : public UiSubobject
{
    T* t = nullptr;

    bool hasObject() const;
    void clear();

  public:
    // Allocate new clone of current game object
    void save(T const&);
    // Assign saved clone to current game object
    void restore(T&) const;

    using UiSubobject::UiSubobject;

    ~Rollback();

    // Would need non-default implementation if used
    Rollback(Rollback const&) = delete;
    Rollback(Rollback&&) = delete;
    Rollback& operator=(Rollback const&) = delete;
    Rollback& operator=(Rollback&&) = delete;
};

} // namespace ui


#include "ui_rollback.inc"


#endif // ROLLBACK_H_INCLUDED
