// ref_afterstate.h
//
// Implementation of RL-style afterstates for more efficient SARSA.
//
// An RL algorithm could identify transitions that lead into terminal states
// and simply use the observed return G instead of querying an afterstate. In
// case an algorithm does not want to bother implementing this special case,
// afterstates also support transitions from and into terminal states.


#ifndef REF_AFTERSTATE_H_INCLUDED
#define REF_AFTERSTATE_H_INCLUDED 1


#include "ref_types.h"

#include "ref_action.h"
#include "ref_state.h"


namespace refac
{

// Afterstate allow us to leverage knowledge about the transition and reward
// functions of an MDP.
// Instances of class "Afterstate" represent afterstates after taking a *legal*
// actions a within a state s.
class Afterstate
{
    // The amount of points in s before the transition (s, a) -> s'.
    // This is necessary to differentiate risk in high-scoring states from risk
    // in low-scoring states.
    Points_t pointsBefore; static_assert(sizeof(pointsBefore) == 4);
    // The amount of points gained immediately by the trhansition (s, a) -> s'.
    // This is necessary to differentiate immediate gain in high-scoring actions
    // from the gain in low-scoring actions.
    Points_t pointsAfter; static_assert(sizeof(pointsAfter) == 4);
    struct
    {
      // The amount of dice that will be present in the next state. They are
      // required (in combination with the points), to determine how dangerous it
      // is to continue play (rather than finishing the episode).
      Count_t diceLeft;
      static_assert(sizeof(diceLeft) == 1);
      // When entering into terminal states, the dice left have no meaning when
      // comparing afterstates.
      unsigned intoTerminal : 1;
      // Entering terminal states with the welp action is different because it
      // zeroes the return. Implies intoTerminal.
      unsigned welping : 1;
      // Special case: Acting in terminal states is indifferent to all the other
      // values.
      unsigned fromTerminal : 1;
    } x; static_assert(sizeof(x) == 4);

    Afterstate() = default; // Indetermined

    static Afterstate createTerminalTransition();
    static Afterstate createFromTransition(State const&, Action const&);

  public:
    Count_t diceLeft() const;
    bool intoTerminal() const;
    bool fromWelp() const;
    bool goodFinish() const;  // Finish deliberately, securing remaining points
    bool badFinish() const;   // Finish by being forced to welp
    bool fromTerminal() const;

    // Compute number of points secured for finishing actions (including welping)
    Points_t pointGain() const;

    // Compare taking into account specia cases when entering the terminal state
    bool operator==(Afterstate const&) const;

    // Construct afterstate from *legal* state-action pair.
    Afterstate(State const&, Action const&);

    friend struct std::hash<refac::Afterstate>;
};
// TODO Can do better than 12 byte if storing/using 2 byte points
static_assert(sizeof(Afterstate) == 12);

} // namespace refac

template<>
struct std::hash<refac::Afterstate>
{
  size_t operator()(refac::Afterstate const& x) const
  {
    return std::hash<int>()
    (
      x.fromTerminal()
      ? ( (int) -1) // Highest bit = from terminal y/n
      : ( (int)x.diceLeft()   <<   0 |
          (int)x.pointsAfter  <<   3 |
          (int)x.pointsBefore <<  15) // reserving 12 bit for < 4096 points
    );
  }
};


#endif // REF_AFTERSTATE_H_INCLUDED
