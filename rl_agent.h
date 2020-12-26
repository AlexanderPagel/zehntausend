#ifndef RL_AGENT_H_INCLUDED
#define RL_AGENT_H_INCLUDED

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
    explicit Agent(rl::Environment* e) : _environment(e)
    {
        if( e == nullptr )
            throw std::invalid_argument("must provide an environment");
    }

    Agent& operator=(Agent const&) = delete;

    Agent(Agent const&) = delete;
    virtual inline ~Agent() = 0;

};

Agent::~Agent(){}


}



#endif // RL_AGENT_H_INCLUDED
