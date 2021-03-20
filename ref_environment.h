// ref_environment.h
//
// Class "Environment" provides the full RL-style environment. User input is
// *not* validated in any way. Actions that are input must be valid in the
// given state. The easiert way to ensure this is to select an action from the
// provided list of legal actions.


#ifndef REF_ENVIRONMENT_H_INCLUDED
#define REF_ENVIRONMENT_H_INCLUDED 1


#include <vector>

#include "ref_action.h"
#include "ref_state.h"
#include "ref_types.h"


namespace refac
{

// Implements the game rules and state transitions.
// For legal inputs only!
class Environment
{
    State state;

    // Provide a list of legal actions at every time step.
    // TODO We might want to change this later, if appropriate. The only
    //      optimization case should be self-play training.
    std::vector<Action> legalActions;

    void clearActions(); // Empty w/o changing cpacity
    void fillActions(); // Refills the actions vector (no extra clear needed)

  public:
    Environment(); // Initialize in a normal start state

    // Take known-legal action, return points gained
    Points_t takeAction(Action const& action); // Return immerdiate reward
    bool episodeFinished() const;
    State const& getState() const;
    std::vector<Action> const& getLegalActions() const;
    void restart();

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

} // namespace refac


#endif // REF_ENVIRONMENT_H_INCLUDED
