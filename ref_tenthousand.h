// ref_tenthousand.h
//
// Provide Class "Tenthousand" implementing the full multiplayer game until 10k
// points.


#ifndef REF_TNETHOUSAND_H_INCLUDED
#define REF_TNETHOUSAND_H_INCLUDED 1


#include <vector>

#include "ref_game.h"
#include "ref_types.h"


namespace refac
{

class Tenthousand
{
  private:
    // TODO state and points separate or in conjunction?
    std::vector<Game> games;
    std::vector<Points_t> savePoints;
    int player{0};
    int winner{noWinner};

    static decltype(winner) constexpr noWinner = -1;

    // Couple helpers for (near) atomic operations for the *current player*
    void addPoints(Points_t points);
    void restartEnvironment();
    void adjustWinner();
    void incrementPlayer();

    template<typename P, typename... Types>
    bool interact(P p, Types... args); // Helper to collect the shared cleanup/checks

    // Internal, non-const version of the player getters
    Game& getGame();
//    GameState& getGameState();
//    State& getState();
//    Cup& getCup();

  public:
    static constexpr int goal = 10000;

    Tenthousand(int playerCount = 0);

    int playerCount() const;
    int activePlayer() const;
    bool hasFinished() const;
    int getWinner() const;

    // Observe game properties
    Game const& getGame() const;
    GameState const& getGameState() const;
    Environment const& getEnvironment() const; // TODO implement
    State const& getState() const;
    Cup const& getCup() const;
    Points_t getPoints() const; // Save player points
    Points_t getPoints(int player) const;

    // Relay game interactions for the *current player*
    bool interactToggleAside(int pos);
    bool interactRoll();
    bool interactFinish();

    // -------------------------------------------------------------------------

    // Stuff that I quickly add as a hack to support the UI
    // requirements.
    void makeMove(Action const&); // TODO I thin not even needed since bot types keys, too (?)
    using Player = int;
    using State_t = State;
    using Move_t = Action;
    bool interactToggleAside(); // put everything aside (return value meaningless currently)
    auto getPlayer() { return activePlayer(); }
    Points_t getCurrent() const;
//    auto getCurrent() { return getState().getPoints(); } // TODO this needs to reflect on the currently in-consruction action, too
    auto getActive() { return getCup().getActive(); }
    auto getUsable(int i) {return getGame().isUsable(i); }
};

} // namespace refac


#endif // REF_TNETHOUSAND_H_INCLUDED
