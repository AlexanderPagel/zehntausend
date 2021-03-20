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
#include "ref_afterstate.h"


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
    static std::vector<Action> generateActions(State const&);
    // Immediate return promised by a *legal* action (throw part only)
    static Points_t pointsWorthRaw(Throw const&);
    // Immediate return considering the turn limit
    static Points_t pointsWorthLimit(Throw const&, Points_t startPoints);
    // Helper to input an entire action
    static Points_t pointsWorthRaw(Action const&);
    static Points_t pointsWorthLimit(Action const&, Points_t startPoints);

    static Points_t constexpr turnLimit{350};

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

    // ---------------------------------------------------------------------------

    // Some dirty hacks that i just throw here to silence the RL algo
    static Afterstate afterstate(State const&, Action const&);
};

} // namespace refac


#endif // REF_ENVIRONMENT_H_INCLUDED
