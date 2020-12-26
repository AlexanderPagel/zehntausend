#ifndef RL_ENVIRONMENT_HPP_INCLUDED
#define RL_ENVIRONMENT_HPP_INCLUDED

#include "rl_action.hpp"
#include "rl_state.hpp"

#include <tuple>
#include <memory>

namespace rl
{

/// side note:
///
/// example other class might be the simulation class
/// which has a nextState (state,action) -> {reward,state} interface
/// but lacks interfaces to get current states, initialize a state and similar
///
/// \note   implementations (e.g., for some games like chess) could inherit both from env. and simul.






class Environment
{
public:
    /// typedefs

    using Reward_t = double;

protected:

public:
    /// ----------------------------------------------------------------
    /// interaction in MDPs --------------------------------------------

    /// \b  Initialize the internal state to a starting state.
    virtual auto
    init()
        -> Environment& = 0;

    virtual auto
    takeAction(Action const&)
        -> Reward_t = 0;

    virtual auto
    cloneState() const
        -> std::shared_ptr<State> = 0;

    Environment() noexcept = default;
    Environment(Environment const&) noexcept = default;
    Environment(Environment&&) noexcept = default;

//    Environment(Environment&& src) = delete;

    virtual  ~Environment() = default;
};

class Simulation
{
    /// types
    using Reward_t = double;    /// \tbc unify with env?
    using Response_t = std::pair<Reward_t,std::shared_ptr<State>>;

    auto
    simulate(State const&, Action const&) const
        -> Response_t;
};


}
#endif // RL_ENVIRONMENT_HPP_INCLUDED
