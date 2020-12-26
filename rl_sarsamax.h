#ifndef RL_SARSAMAX_H_INCLUDED
#define RL_SARSAMAX_H_INCLUDED

#include "rl_agent.h"

// debug
#include "tenthousand.h"

using ::from_rl_bases::TenKState;

namespace rl
{


class SarsaMax : public Agent, private Policy
{
protected:
    double const    _alpha,
                    _gamma;

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
    update( State const& s1, Action const& a1, double r,
            State const& s2)
        -> void
    {
        ::from_rl_bases::TenKState const& tempS1( dynamic_cast<::from_rl_bases::TenKState const&>(s1));
        ::from_rl_bases::TenKState const& tempS2( dynamic_cast<::from_rl_bases::TenKState const&>(s2));
        double oldEstimate = getEstimate(s1,a1);
        double delta = r + _gamma*greedyValue(s2) - oldEstimate;
        double newEstimate = oldEstimate + _alpha*delta;
        setEstimate(s1, a1, newEstimate);
    }

    virtual auto
    greedyValue(State const&) const
        -> double = 0;

public:
    void performEpisode()
    {
        _environment->init();                                               // Initialize S
        shared_ptr<State const> s1 = _environment->cloneState();            //  .
        while( s1->isTerminal() == false )                                  // Repeat for each step of the episode
        {                                                                   //

            ::from_rl_bases::TenKState const& tempS( dynamic_cast<::from_rl_bases::TenKState const&>(*s1));
            if(
               tempS.thrown()[0]==0&&
               tempS.thrown()[1]==1&&
               tempS.thrown()[2]==0&&
               tempS.thrown()[3]==0&&
               tempS.thrown()[4]==3&&
               tempS.thrown()[5]==0&&
               tempS.points() == 200
               )
                cout << setw(0);

            shared_ptr<Action const> a1 = this->operator()(*s1);            // Choose A from S using policy derived from Q
            ::from_rl_bases::TenKMove const& tempA( dynamic_cast<::from_rl_bases::TenKMove const&>(*a1));

            if(
//               tempA.putAside()[0]==0&&
//               tempA.putAside()[1]==0&&
//               tempA.putAside()[2]==0&&
//               tempA.putAside()[3]==0&&
//               tempA.putAside()[4]==3&&
//               tempA.putAside()[5]==0&&
               tempA.finishes()==true
               )
                cout << setw(0);

            double r = this->Agent::_environment->takeAction(*a1);          // Take action A, observe R,
            shared_ptr<State const> s2 = _environment->cloneState();        //  S'

            ::from_rl_bases::TenKState const& tempS2( dynamic_cast<::from_rl_bases::TenKState const&>(*s2));

            update(*s1,*a1,r,*s2);                                          // Update towards R + gamma* max_a{ Q(s, a) }
            s1 = s2;                                                        // S <- S'; A <- A'
            if( s1->isTerminal() )
                cout << setw(0);    // check after how many states this occurs
            // and then check if the reward is negative upon losing.
        }                                                                   // until S is terminal

    }   // train function

    SarsaMax(Environment* e, double alp, double gam)
        : Agent::Agent(e), _alpha(alp), _gamma(gam) {}
    virtual inline ~SarsaMax() = 0;
};

SarsaMax::~SarsaMax(){}

}

#endif // RL_SARSAMAX_H_INCLUDED
