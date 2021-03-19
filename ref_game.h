// ref_game.h
//
// Provide class "Game":
//
// Game-like (incremental) and sanity-checked implementation of the (episodic)
// one-player game. This class will reject actions in a terminal state as
// illegal.
// Only the game-related members (toggle, roll, finish) are sanity checked
// (returning a boolean indicating success). When these interactions fail, they
// are no-ops. That means, the object can still be used afterwards as if the
// erroneous request had not been made.
// Non-game-related members like getReturn() and the private member functions
// are *not* sanity checked. Member getReturn() MUST only be called in terminal
// states.


#ifndef REF_GAME_H_INCLUDED
#define REF_GAME_H_INCLUDED 1


namespace refac
{

class Game
{
    GameState gameState;
    Selection_t usable;  // Not yet scored dice

    bool scanLegalActions(Action const&) const; // Return true if action is legal
    void adjustUsable(); // Set usable <- current active dice
    void takeConstructedAction(); // Take action and adjust the usable vector to new state

    // Helper because roll() and finish() are almost identical otherwise.
    // Returns false if intended action is illegal (can be passed as return
    // from roll() and finish()).
    bool endSubturnWith(bool finish);

  public:
    Game(); // Initialize new game

    bool isTerminal() const; // Terminal = no further interactions allowed
    Points_t getReturn() const; // Get episode return in terminal states

    // Interactions return "true" if the interaction is successfull, otherwise,
    // they return "false" and might leave the object in a corrupted state. In
    // this case, restart() must be called before using the object otherwise.
    bool toggleAside(int pos);
    bool roll();    // Roll remaining dice
    bool finish();  // Finish with current selection

    // Initialize new game. Recovers from corrupted states.
    void restart();
};

} // namespace refac


#endif // REF_GAME_H_INCLUDED
