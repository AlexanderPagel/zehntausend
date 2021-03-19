// ref_tenthousand.h
//
// Provide Class "Tenthousand" implementing the full multiplayer game until 10k
// points.


#ifndef REF_TNETHOUSAND_H_INCLUDED
#define REF_TNETHOUSAND_H_INCLUDED 1


namespace refac
{

class Thenthousand
{
  private:
    // TODO state and points separate or in conjunction?
    std::vector<gameState> gameStates;
    std::vector<Points_t> savePoints;
    int player{0};
    int winner{noWinner};

    static decltype(winner) constexpr noWinner = -1;

    // Couple helpers for (near) atomic operations for the *current player*
    void addPoints(Points_t points);
    void restartEnvironment();
    void adjustWinner();
    void incrementPlayer();

    template<GameState::* Member, typename... Types>
    bool interact(Types... args); // Helper to collect the shared cleanup/checks

  public:
    static constexpr int goal = 10000;

    Tethousand(int playerCount = 0);

    int playerCount() const;
    int activePlayer() const;
    bool hasFinished() const;
    int winner() const;

    // Observe game properties
    GameState const& getGameState() const;
    State const& getState() const;
    Cup const& getCup() const;
    Points_t getPoints() const; // Save player points
    Points_t getPoints(int player) const;

    // Relay game interactions for the *current player*
    bool interactToggleAside(int pos);
    bool interactRoll();
    bool interactFinish();
};

} // namespace refac


#endif // REF_TNETHOUSAND_H_INCLUDED
