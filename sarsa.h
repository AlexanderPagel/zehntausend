#ifndef SARSA_HPP_INCLUDED
#define SARSA_HPP_INCLUDED


#include <array>
#include <unordered_map>
//#include "state.h"
//#include "tenK.h"
#include <iostream>
#include <iomanip>
#include <limits>

#include "ref_tenthousand.h"
#include "ref_afterstate.h"


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
    using _game_t        = refac::Environment;
//    using Environment_t  = _game_t;
    using Environment_t  = refac::Environment;
public:
    using Game_t         = _game_t;
    using State_t        = refac::State;
    using Action_t       = refac::Action;
//    using Afterstate_t   = Game_t::Afterstate_t;
    using Afterstate_t   = refac::Afterstate; // TODO implement
//    using ActionVector_t = Game_t::ActionVector_t;
    using ActionVector_t = std::vector<refac::Action>;
    using Reward_t = refac::Points_t;
    using Return_t = Reward_t;

private:
    using _legalActionsTable_t = std::unordered_map<State_t, ActionVector_t>;
    using _afterstateTable_t = std::unordered_map<Afterstate_t, double>;

public:
    double const alpha;
    double const epsilon;
    double const gamma;

    Environment_t e{};

    mutable _afterstateTable_t _afterstateValueTable;   // mutable for greedy lookup, not training
    mutable _legalActionsTable_t _legalActionsTable;

    // Use environment to compute legal actions and store them
    double  _afterstateValueLookup(Afterstate_t const& as) const; /// \tbc this also can buffer finishing states?
    double& _afterstateValueUpdate(Afterstate_t const& as) const;
    ActionVector_t const& _legalActionsLookup(State_t const&) const;

public:
    Action_t const& greedy (State_t const& s, bool v=false) const;   // rvalue obj versions?
    Action_t const& eGreedy(State_t const& s) const;   // rvalue obj versions?

public:
    void performLearningEpisodes(
        unsigned int n = 1,
        unsigned int l = 0,
        std::ostream& = std::cout);
    // Teturn episode return, may need to convert to larger type
    Return_t performLearningEpisode();

    // Ctor
    Sarsa(double alpha = 0.1, double epsilon = 0.2, double gamma = 1);

//    void printLowQ() const & noexcept;
};


#endif // SARSA_HPP_INCLUDED
