#ifndef RL_EXCEPTIONS_H_INCLUDED
#define RL_EXCEPTIONS_H_INCLUDED

#include <stdexcept>

namespace rl
{

class UninitializedError : public std::runtime_error
{
public:
    UninitializedError(std::string s) : runtime_error(s) {}
};

/// some form of error in the MDP process: impossible action selected, ...
class MdpError : public std::runtime_error
{

};


class InvalidMoveError : public MdpError
{

};


}

#endif // RL_EXCEPTIONS_H_INCLUDED
