#include "rl_agent.h"

#include <cassert>

namespace rl
{


Agent::Agent(rl::Environment* e)
  : _environment(e)
{
  assert(e != nullptr);
}


Agent::~Agent() {}


} // namespace rl
