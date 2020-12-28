#ifndef RL_ACTION_HPP_INCLUDED
#define RL_ACTION_HPP_INCLUDED

namespace rl
{


class Action
{

    /// Representation completely implementation-dependent.

public:
//    virtual Action& operator=(Action&&) = 0;
//    virtual Action& operator=(Action const&) = 0;
//    Action(Action&&) noexcept = delete;
    Action() = default;
    Action(Action const&) = default;
    Action(Action&&) = default;

    virtual ~Action() = 0;
};


}

#endif // RL_ACTION_HPP_INCLUDED
