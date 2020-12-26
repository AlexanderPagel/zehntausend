#ifndef RL_AGENT_HPP_INCLUDED
#define RL_AGENT_HPP_INCLUDED

#include "rl_environment.hpp"
#include "rl_policy.hpp"

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



#endif // RL_AGENT_HPP_INCLUDED
