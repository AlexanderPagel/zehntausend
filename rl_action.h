#ifndef RL_ACTION_H_INCLUDED
#define RL_ACTION_H_INCLUDED 1

namespace rl
{


class Action
{

    /// Representation completely implementation-dependent.

public:
//    virtual Action& operator=(Action&&) = 0;
//    virtual Action& operator=(Action const&) = 0;
//    Action(Action&&) noexcept = delete;
//    Action() = default;
//    Action(Action const&) = default;
//    Action(Action&&) = default;

    virtual ~Action() = 0;
};


} // namespace rl

#endif // RL_ACTION_H_INCLUDED
