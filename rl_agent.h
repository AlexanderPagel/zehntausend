#ifndef RL_AGENT_HPP_INCLUDED
#define RL_AGENT_HPP_INCLUDED

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
    //    Policy* _policy;/// \tbc ????

public:
    explicit Agent(rl::Environment* e) : _environment(e)
    {
        if( e == nullptr )
            throw invalid_argument("must provide an environment");
    }

    Agent& operator=(Agent const&) = delete;

    Agent(Agent const&) = delete;
    virtual ~Agent();

};


}



#endif // RL_AGENT_HPP_INCLUDED
