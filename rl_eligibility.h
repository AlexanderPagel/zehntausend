#ifndef RL_ELIGIBILITY_HPP_INCLUDED
#define RL_ELIGIBILITY_HPP_INCLUDED

#include <unordered_map>
#include <iterator>

#include "rl_afterstate.hpp"

namespace rl
{


/// Mean to be used in classes derived from (e.g.) Sarsa to instantiate
///  either as member or statically in update function
///
class Eligibility
{
public:
    using Elig_t = double;

protected:
    virtual auto stimulate(State const&) -> void = 0;
    /// Might be redefined in derived classes to set e to 1 (not add 1)
    auto decay() -> void;

public:
    virtual void visit(State const&) = 0;
    virtual auto init() -> void = 0;

    Eligibility() = default;
    virtual inline ~Eligibility() = 0;
};

Eligibility::~Eligibility() {}

} //  ::rl

#endif // RL_ELIGIBILITY_HPP_INCLUDED
