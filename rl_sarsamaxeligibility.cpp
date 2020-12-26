#include "rl_sarsamaxeligibility.hpp"

namespace rl
{

auto
SarsaMaxLambda::performEpisode()
-> void
{
    _environment->init();
    _eligTraces->init();
    std::shared_ptr<State const> s1( _environment->cloneState() );
    while( !s1->isTerminal() )
    {
        std::shared_ptr<Action const> a1 = this->operator()(*s1);
        double r = this->Agent::_environment->takeAction(*a1);
        std::shared_ptr<State const> s2 = _environment->cloneState();
        std::shared_ptr<Action const> a2 = this->operator()(*s2);
        update(*s1,*a1,r,*s2,*a2);
        s1 = s2;
    }
} // train function


}


