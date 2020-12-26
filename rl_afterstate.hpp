#ifndef RL_AFTERSTATE_HPP_INCLUDED
#define RL_AFTERSTATE_HPP_INCLUDED

#include "rl_state.hpp"
#include "rl_action.hpp"

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

#endif // RL_AFTERSTATE_HPP_INCLUDED
