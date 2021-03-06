// ui_types.h
//
// Common types in UI classes

#ifndef UI_TYPES_INCLUDED
#define UI_TYPES_INCLUDED 1


#include "ref_tenthousand.h"


namespace ui
{

using Game_t = refac::Tenthousand;
// The player number (3) does not matter for the remaining types
using Player_t = Game_t::Player;
using State_t = Game_t::State_t;
using Points_t = refac::Points_t; // TODO State::Points_t?
using Action_t = Game_t::Move_t;

} // namespace ui


#endif // UI_TYPES_INCLUDED
