#ifndef SARSA_H_INCLUDED
#define SARSA_H_INCLUDED

#include <array>
#include <unordered_map>
#include "tenthousand.h"
//#include <conio.h> // getch()
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>

#include "rl_headers.h"
#include "tenk_elig.h"

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

//class Sarsa;
//template class Tenthousand<1>; /// \tbc ? why is this here? :D

/// \tbc template with type parameter derived from environment
class Sarsa
{
private:
    /// ------------------------------------------------------------------------
    /// types
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

public:
    double const _epsilon;    // e.g., 10 for epsilon = 0.1 /// \tbc maybe change to actual epsilon
    double const _alpha;
    double const _gamma;

    mutable _afterstateTable_t _afterstateValueTable;   // mutable for greedy lookup, not training
    mutable _legalActionsTable_t _legalActionsTable;

protected:
    double  _afterstateValueLookup(Afterstate_t const& as) const; /// \tbc this also can buffer finishing states?
    double& _afterstateValueUpdate(Afterstate_t const& as) const;
    ActionVector_t const& _legalActionsLookup(State_t const&) const;  // rvalue version should not insert if not found

public:
    /// ------------------------------------------------------------------------
    /// Game api
    Action_t greedy(State_t const& S, bool v=false) const;   // rvalue obj versions?
    Action_t eGreedy(State_t const& S) const;   // rvalue obj versions?
    void performLearningEpisodes(unsigned int n = 1, unsigned int l = 0, std::ostream& = cout) &;

protected:
    /// ------------------------------------------------------------------------
    /// rl::SarsaMaxLambda requirements
    auto getEstimate(State_t const& s, Action_t const& a) const -> double;
    auto setEstimate(State_t const& s, Action_t const& a, double g) -> void;
    auto greedyValue(State_t const& s) const -> double;
    auto update(State_t const&, Action_t const&, double,
                State_t const&, Action_t const&) -> void;

    /// ------------------------------------------------------------------------
    /// rl::Policy requirements
    auto operator()(rl::State const& s) const -> std::shared_ptr<rl::Action>;

public:
    /// ------------------------------------------------------------------------
    /// ctors/dtor
    Sarsa(_game_t* env, double alph=.1, double epsil=.1, double gam=.99, double lamb=.9) noexcept;
    ~Sarsa();

};


#endif // SARSA_H_INCLUDED
