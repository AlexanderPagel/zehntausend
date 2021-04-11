// ref_state.h
//
// Class "State" providing an RL-style state representation, including thrown
// dice and the episode points.


#ifndef REF_STATE_H_INCLUDED
#define REF_STATE_H_INCLUDED 1


#include "ref_throw.h"
#include "ref_types.h"


namespace refac
{

class State
{
    // - current points
    // - current dice
    // - unambiguous representation for terminal states

    Throw thrown{}; // TODO derive for operator-= etc?
                    //      Also for empty()
    Points_t points{0};
    bool terminal{false};

    State() = default; // Hopefully uses the in-class initializers
    State(Throw const&, Points_t const&);

  public:
    static State startState(); // Randomize 6 dice, points 0
    static State startState(Points_t maxPoints); // Radnom points in [0,2000]
    static State makeTerminalState();

    bool isTerminal() const;
    bool operator==(State const& other) const;
    Points_t getPoints() const;
    Throw const& getThrown() const;
    void addPoints(Points_t);
    void setTerminal(bool t = true);
    // NOTE Behaves different to Throw::roll()
    void roll(Count_t n); // Randomize remaining dice, or 'n' new if 0 remaining

    // Remove some dice without altering points
    void operator-=(Throw const&);

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

} // namespace refac


#include "ref_state.inc"


#endif // REF_STATE_H_INCLUDED
