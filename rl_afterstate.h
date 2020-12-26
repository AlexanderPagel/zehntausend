#ifndef RL_AFTERSTATE_H_INCLUDED
#define RL_AFTERSTATE_H_INCLUDED

#include "rl_state.h"
#include "rl_action.h"

namespace rl
{


class Afterstate : public State
{
public:
    /// \tbc    ctor from state and action, maybe factory

    virtual auto
    fromSA(State const&, Action const&) &
        -> Afterstate& = 0;
};


}

#endif // RL_AFTERSTATE_H_INCLUDED
