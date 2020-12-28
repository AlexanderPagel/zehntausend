#ifndef TD_PREDICTION_HPP_INCLUDED
#define TD_PREDICTION_HPP_INCLUDED

#include <array>
#include <unordered_map>
#include "state.hpp"

// later for policy with decisions to put dice aside
unsigned int constexpr thrwosWith6 = 462;
unsigned int constexpr thrwosWith5 = 252;
unsigned int constexpr thrwosWith4 = 126;
unsigned int constexpr thrwosWith3 = 56;
unsigned int constexpr thrwosWith2 = 21;
unsigned int constexpr thrwosWith1 = 6;
unsigned int constexpr thrwos = 923; // sum of the above
//unsigned int constexpr numberOfStates = throws * 2000/50 +1; // +1 for 0 points

// now for decisions about continue or not while putting everything aside:
unsigned int constexpr numberOfStates = (2000/50+1) * 7; // including points == 0 and dice number == 0

namespace std
{
    template <> struct hash<State>
    {
        size_t operator()(const State& x) const
        {
            return std::hash<unsigned long long>()( ((unsigned long long) x.points << 32) + x.dice); /// \tbc alternatively reserve 4 bits for dice and 28 bits for points
        }
    };
}


class TDPrediction
{
    std::unordered_map<State, double> V;    // estimate for v_Pi





};

#endif // TD_PREDICTION_HPP_INCLUDED




#ifndef VALUE_ITERATION_HPP_INCLUDED
#define VALUE_ITERATION_HPP_INCLUDED


#endif // VALUE_ITERATION_HPP_INCLUDED
