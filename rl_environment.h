#ifndef RL_ENVIRONMENT_HPP_INCLUDED
#define RL_ENVIRONMENT_HPP_INCLUDED

#include <tuple>

namespace rl
{


class Environment
{
public:
    /// typedefs
    using State_t = rl::State;
    using Action_t = rl::Action;
    using Reward_t = double;
private:
    /// private members
    State_t _s = {0};
public:
    using Resonse_t = std::pair<Reward_t, State_t const&>;

    /// interaction in MDPs
    Response_t takeAction( const Action_t& ) = 0;


    virtual  ~Environment() = 0;
};

class Simulation
{
    /// typedefs
    using State_t = rl::State;
    using Action_t = rl::Action;
    using Reward_t = double;    /// \tbc unify with env?

    void simulate(State_t const&, Action_t const&, Reward_t& r, State_t&);
};


}
#endif // RL_ENVIRONMENT_HPP_INCLUDED
