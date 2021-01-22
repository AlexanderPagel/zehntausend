#ifndef SARSA_HPP_INCLUDED
#define SARSA_HPP_INCLUDED

#include <array>
#include <unordered_map>
//#include "state.h"
//#include "tenK.h"
#include "tenK.h"
#include "tenKMove.h"
#include "tenKState.h"
#include <iostream>
#include <iomanip>
#include <limits>

// debug
//static unsigned int d_count = 0;

//// later for policy with decisions to put dice aside
//unsigned int constexpr thrwosWith6 = 462;
//unsigned int constexpr thrwosWith5 = 252;
//unsigned int constexpr thrwosWith4 = 126;
//unsigned int constexpr thrwosWith3 = 56;
//unsigned int constexpr thrwosWith2 = 21;
//unsigned int constexpr thrwosWith1 = 6;
//unsigned int constexpr thrwos = 923; // sum of the above
////unsigned int constexpr numberOfStates = throws * 2000/50 +1; // +1 for 0 points
//
//// now for decisions about continue or not while putting everything aside:
//unsigned int constexpr numberOfStates = (2000/50+1) * 7; // including points == 0 and dice number == 0

class Sarsa;
template class Tenthousand<1>;

/// \tbc template with type parameter derived from environment
class Sarsa
{
private:
    // If we implement another game variant we only need to change typedef for
    // our type "_game_t" and everything else is corrected implicitly.
    // We have some (implicit) expectations on our game type, for example:
    //  - provide types for state, action, ...
    //  - provide function to compute legal actions
    //  - provide function to construct afterstates
    //  - ...
    using _game_t        = Tenthousand<1>;
    using Environment_t  = _game_t;
public:
    using Game_t         = _game_t;
    using State_t        = Game_t::State_t;
    using Action_t       = Game_t::Move_t;
    using Afterstate_t   = Game_t::Afterstate_t;
    using ActionVector_t = Game_t::ActionVector_t;

private:
    using _legalActionsTable_t = std::unordered_map<State_t, ActionVector_t>;
    using _afterstateTable_t = std::unordered_map<Afterstate_t, double>;

public:
    double const alpha;
    double const epsilon;    // Store as 1/epsilon, e.g., 10 for epsilon = 0.1 // TODO
    double const gamma;

    mutable _afterstateTable_t _afterstateValueTable;   // mutable for greedy lookup, not training
    mutable _legalActionsTable_t _legalActionsTable;

    // Use environment to compute legal actions and store them
    double  _afterstateValueLookup(Afterstate_t const& as) const; /// \tbc this also can buffer finishing states?
    double& _afterstateValueUpdate(Afterstate_t const& as) const;
    ActionVector_t const& _legalActionsLookup(State_t const&) const;

public:
    Action_t greedy (State_t const& s, bool v=false) const;   // rvalue obj versions?
    Action_t eGreedy(State_t const& s) const;   // rvalue obj versions?

public:
    void performLearningEpisodes(
        unsigned int n = 1,
        unsigned int l = 0,
        std::ostream& = std::cout);

    // Ctor
    Sarsa(double alpha = 0.1, double epsilon = 0.2, double gamma = 1);

//    void printLowQ() const & noexcept;
};


#endif // SARSA_HPP_INCLUDED
