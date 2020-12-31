// ui_types.h
//
// Common types in UI classes

#ifndef UI_TYPES_INCLUDED
#define UI_TYPES_INCLUDED 1


#include "tenthousand.h"


// FIXME Provide namespace
namespace ui
{

using Game_t = Tenthousand<3>;
using Player_t = Game_t::Player;
using State_t = Game_t::State_t;
using Points_t = State_t::Points_t;

} // namespace ui


#endif // UI_TYPES_INCLUDED
