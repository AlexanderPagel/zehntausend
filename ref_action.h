// ref_action.h
//
// Provide class "Action" representing an RL-style action. That means, it
// selects an (unordered) set of dice to be scored, and a boolean deciding
// whether or not to finish the episode, or continue for another sub-turn.

#ifndef REF_ACTION_H_INCLUDED
#define REF_ACTION_H_INCLUDED 1


#include "ref_throw.h"
#include "ref_types.h"


namespace refac
{

// Class Action represents a "raw" action. That means, all the information that
// a RL angent would use to interact with the game.
// TODO it might be faster to encode the resulting points in the
//      Action object rather than computing them separately. However,
//      that would somewhat break our nice RL setup.
class Action
{
    // TODO need to encapsulate like the "Throw" class?

  public:
    // Represent "none" action by the default values:
    //  - t = {0}
    //  - finish = false
    // This pattern can never represent any (other) legal action.
    Throw taking{};  // Throw subset that is to be scored
    bool finish{false}; // Decide whether to re-roll or finish

    static Action makeNone();
    static Action makeWelp();

    Action() = default;
    template<typename T, typename U>
    Action(T&& t, U&& u) : taking(std::forward<T>(t)), finish(std::forward<U>(u)) {}
    // TODO need "resign" action when an episode is a forced loss?

    // Test for virtual "none" action that is used when there are no
    // legal actions to transition to the terminal state with
    // 0 reward.
    bool isNone() const;
    bool isWelp() const;
    bool operator==(Action const& other) const;

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

} // namespace refac


#endif // REF_ACTION_H_INCLUDED
