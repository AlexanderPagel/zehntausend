#ifndef RL_POLICY_H_INCLUDED
#define RL_POLICY_H_INCLUDED 1

#include <memory>

#include "rl_state.h"
#include "rl_action.h"

namespace rl
{


// Interface for sample-able policies. When applied returns sampled action (not
// distribution over all actions).
class Policy
{

public:
  virtual auto operator()(State const&) const
    -> std::shared_ptr<Action> = 0;

  virtual ~Policy() = 0;
};


} // namespace rl

#endif // RL_POLICY_H_INCLUDED
