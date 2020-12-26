#ifndef RL_ACTION_H_INCLUDED
#define RL_ACTION_H_INCLUDED

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

    virtual inline ~Action() = 0;
};

Action::~Action() {}


}

#endif // RL_ACTION_H_INCLUDED
