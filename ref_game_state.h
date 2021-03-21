// ref_game_state.h
//
// Implement class "GameState" that is the internal representation of
// a game-like state. Inputs MUST be suitable to the situation, i.e., represent
// legal actions for the represented game.
//
// Dice that have been scored in a previous subturn may not be used again
// during (during the episode). However, dice that have just been put aside can
// be toggled freely between aside and not aside.


#ifndef REF_GAME_STATE_H_INCLUDED
#define REF_GAME_STATE_H_INCLUDED 1


#include "ref_action.h"
#include "ref_cup.h"
#include "ref_environment.h"


namespace refac
{

class GameState
{
    // Internally run a normal Simulation in the 'environment' object. After
    // each change, adjust the cup by-hand to stay consistent with the new
    // state.
    Environment environment;
    Cup cup;
    Action action{Action::makeWelp()};  // Action in construction

    // Replace active dice by a random permutation of the argument. Makes
    // member 'cup' consistent to the member 'state' as long as the number of
    // active dice in cup matches the number of dice in the state.
    void makeDigitsConsistent();
    // Resizes cup based on environment. Used when environment state gets new
    // dice by rolling after putting every (remaining) die aside. Overwrites
    // all digits; call makeDigitsConsistent() afterwards.
    void cupResize();

  public:
    GameState();

    // Internal consistency check
    bool consistent() const; // TODO needed for debugging?

    // Get game information
    Environment const& getEnvironment() const;
    Cup    const& getCup   () const;
    State  const& getState () const;  // Disregard dice put aside in current turn
    Action const& getAction() const;  // Make sure to set action beforehand
    bool isTerminal() const;

    // Interact with game
    void toggleAside(int pos);
    void selectActionFinish(bool finish);
    void finishTurn(); // Call selectActionFinish beforehand
    void finishTurn(bool finish);
    void restart(); // Initialize new game

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

} // namespace refac


#endif // REF_GAME_STATE_H_INCLUDED
