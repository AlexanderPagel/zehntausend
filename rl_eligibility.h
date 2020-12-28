#ifndef RL_ELIGIBILITY_H_INCLUDED
#define RL_ELIGIBILITY_H_INCLUDED 1

#include <unordered_map>
#include <iterator>

#include "rl_afterstate.h"

namespace rl
{


// Meant to be used in classes derived from (e.g.) Sarsa to instantiate either
// as member or statically in update function
class Eligibility
{
public:
    // TODO decying in abstract base class?
    using Elig_t = double;

protected:
    virtual auto stimulate(State const&) -> void = 0;
    /// Might be redefined in derived classes to set e to 1 (not add 1)
    virtual auto decay() -> void = 0;

public:
    virtual void visit(State const&) = 0;
    virtual auto init() -> void = 0;

    virtual ~Eligibility() = 0;
};

} //  ::rl

#endif // RL_ELIGIBILITY_H_INCLUDED
