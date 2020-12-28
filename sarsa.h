#ifndef SARSA_HPP_INCLUDED
#define SARSA_HPP_INCLUDED

#include <array>
#include <unordered_map>
//#include "state.h"
#include "tenthousand.h"
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
class Sarsa //: // public rl::SarsaMax
{
private:
    typedef Tenthousand<1> _game_t; /// \tbc make this template?
    typedef _game_t Environment_t;  /// \tbc environment base class?
public:
    typedef _game_t Game_t;

    typedef Game_t::State_t State_t;    /// \tbc make all template-resistenat and stuff
    typedef Game_t::Move_t Action_t;
    typedef Game_t::Afterstate_t Afterstate_t;

    typedef Game_t::ActionVector_t ActionVector_t;

private:
    typedef std::unordered_map<State_t,Game_t::ActionVector_t> _legalActionsTable_t;
    typedef std::unordered_map<Afterstate_t, double> _afterstateTable_t;
    /// \state restrictions go here (as lambda/function that is applied during lookup to transform the state/afterstate into the desired form, e.g., redustd::cing points to 2000 max)

public:

    double const alpha;
    double const epsilon;    // e.g., 10 for epsilon = 0.1 /// \tbc maybe change to actual epsilon
    double const gamma;

    mutable _afterstateTable_t _afterstateValueTable;   // mutable for greedy lookup, not training
    mutable _legalActionsTable_t _legalActionsTable;

    double _afterstateValueLookup(Afterstate_t const& as) const; /// \tbc this also can buffer finishing states?
    double& _afterstateValueUpdate(Afterstate_t const& as) const;
    ActionVector_t const& _legalActionsLookup(State_t const&) const;  // rvalue version should not insert if not found

public:
    Action_t greedy(State_t const& S, bool v=false) const;   // rvalue obj versions?
    Action_t eGreedy(State_t const& S) const;   // rvalue obj versions?

private:

public:
    void performLearningEpisodes(unsigned int n = 1, unsigned int l = 0, std::ostream& = std::cout) &;

    // :test
    Sarsa(double alpha = 0.1, double epsilon = 0.2, double lambda = 1) noexcept;

//    void printLowQ() const & noexcept;
};


#endif // SARSA_HPP_INCLUDED
