#ifndef RL_AGENT_H_INCLUDED
#define RL_AGENT_H_INCLUDED 1

#include "rl_environment.h"
#include "rl_policy.h"

#include <memory>

namespace rl
{

//class Policy??

class Agent
{
protected:
    Environment* _environment;
    //    Policy* _policy;/// \tbc ???? (related to the note underneath)

public:
    /// \tbc function to take an action ^^
    explicit Agent(rl::Environment* e);

    virtual ~Agent() = 0;
};


} // namespace rl



#endif // RL_AGENT_H_INCLUDED
