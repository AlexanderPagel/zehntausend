#ifndef RL_POLICY_H_INCLUDED
#define RL_POLICY_H_INCLUDED

namespace rl
{


class Policy
{

public:
    virtual auto
    operator()(State const&) const
        -> std::shared_ptr<Action> = 0;

    virtual inline ~Policy() = 0;
};

Policy::~Policy(){}


}

#endif // RL_POLICY_H_INCLUDED
