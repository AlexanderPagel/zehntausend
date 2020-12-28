#ifndef RL_STATE_HPP_INCLUDED
#define RL_STATE_HPP_INCLUDED

/// \support moves?

namespace rl
{


class State
{
    public:
    /// \return True when state is representing the (artificially
    ///          constructed) terminal state.
    /// \note   May be implemented as boolean member variable or as function
    ///          of the internal state representation.
    bool isTerminal() const;

    /// move-requirements
    explicit State(State&&) = 0;
    State& operator=(State&&) = 0;

    virtual ~State();
};


}


#endif // RL_STATE_HPP_INCLUDED
