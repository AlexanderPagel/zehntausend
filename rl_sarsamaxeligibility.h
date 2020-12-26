#ifndef RL_SARSAMAXELIGIBILITY_H_INCLUDED
#define RL_SARSAMAXELIGIBILITY_H_INCLUDED

#include <memory>

#include "rl_agent.h"
#include "rl_policy.h"
#include "rl_eligibility.h"

namespace rl
{


class SarsaMaxLambda : public Agent, private Policy /// \tbc incorp eligibility
{
protected:
    double const    _alpha,
                    _gamma;
    Eligibility* _eligTraces;

protected:
    /// \note   State-action values might be implemented with afterstates
    ///          (by converting s,a into an afterstate type)
    virtual auto
    getEstimate(State const&, Action const&) const
        -> double = 0;  // (dynamic_)cast to instantiated type in here and ensure the environment provides the correct one

    virtual auto
    setEstimate(State const&, Action const&, double g)
        -> void = 0;

    virtual auto
    update(rl::State const& s1, Action const& a1, double r, State const& s2, rl::Action const& a2)
        -> void = 0;

    virtual auto
    greedyValue(State const&) const
        -> double = 0;

public:
    void performEpisode();

    SarsaMaxLambda(rl::Environment* env, double alp, double gam, Eligibility* elig)
        : Agent::Agent(env), _alpha(alp), _gamma(gam), _eligTraces(elig) {}
    virtual inline ~SarsaMaxLambda() = 0;

    /// deleted functions
    SarsaMaxLambda& operator=(SarsaMaxLambda const&) = delete;
    SarsaMaxLambda(SarsaMaxLambda const&) = delete;
};

SarsaMaxLambda::~SarsaMaxLambda()
{
    // pointer is constructed outside of the object and passed to constructor
    // it should be destructed outside
}

}


#endif // RL_SARSAMAXELIGIBILITY_H_INCLUDED
