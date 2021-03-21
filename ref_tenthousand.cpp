#include "ref_tenthousand.h"


namespace refac
{

void
Tenthousand::addPoints(Points_t points)
{
  assert(0 <= points);
  savePoints[player] += points;
}

void
Tenthousand::restartEnvironment()
{
  games[player].restart();
}

void
Tenthousand::adjustWinner()
{
  assert(!hasFinished());
  if(getPoints() >= goal)
    winner = player;
}

void
Tenthousand::incrementPlayer()
{
  ++player;
  if (player == playerCount())
    player = 0;
}

// This helper collects the cleanup and sanity checks required for interactions
// (- Assert that game is still going, debug only)
//  - Adjust player points if episode ends
//  - Restart environment if episode ends
//  - Determine winner if player reaches 'goal' points
//  - Increment player if episode ends
// @p p Pointer to member function of class "Game" that is to be called
template<typename P, typename... Types>
bool
Tenthousand::interact(P p, Types... args)
{
  assert(!hasFinished());

  // Class "GameState" ensures that game interactions are nops when failing.
  // Then, cleanup is unnecessary since the game states are just like before.
  auto& g = getGame();
  auto success{(g.*p)(args...)};
  if(success == false)
    return false;

  // If episode was finished, we need some cleanups
  if (g.isTerminal())
  {
    addPoints(g.getReturn());
    g.restart();
    adjustWinner();
    incrementPlayer(); // Must be last because is changes current player
  }

  return true;
}

Game&
Tenthousand::getGame()
{
  return games[player];
}

//GameState&
//Tenthousand::getGameState()
//{
//  return getGame().getGameState();
//}

//State&
//Tenthousand::getState()
//{
//  return getGameState().getState();
//}

//Cup&
//Tenthousand::getCup()
//{
//  return getGameState().getCup();
//}

Tenthousand::Tenthousand(int playerCount)
  : games(playerCount),
    savePoints(playerCount, 0)
{}

int
Tenthousand::playerCount() const
{
  assert(games.size() == savePoints.size());
  return static_cast<int>(games.size());
}

int
Tenthousand::activePlayer() const
{
  assert(!hasFinished());
  return player;
}

bool
Tenthousand::hasFinished() const
{
  return winner != noWinner;
}

int
Tenthousand::getWinner() const
{
  assert(hasFinished());
  return winner;
}

Game const&
Tenthousand::getGame() const
{
  return games[player];
}

GameState const&
Tenthousand::getGameState() const
{
  return getGame().getGameState();
}

State const&
Tenthousand::getState() const
{
  return getGameState().getState();
}

Cup const&
Tenthousand::getCup() const
{
  return getGameState().getCup();
}

Points_t
Tenthousand::getPoints() const
{
  return getPoints(player);
}

Points_t
Tenthousand::getPoints(int player) const
{
  return savePoints.at(player);
}

bool
Tenthousand::interactToggleAside(int pos)
{
  return interact(&Game::toggleAside, pos);
}

bool
Tenthousand::interactRoll()
{
  return interact(&Game::roll);
}

bool
Tenthousand::interactFinish()
{
  return interact(&Game::finish);
}

// -----------------------------------------------------------------------------

bool
Tenthousand::interactToggleAside()
{
  return interact(&Game::toggleMax);
}

Points_t
Tenthousand::getCurrent() const
{
  return getState().getPoints()
       + Environment::pointsWorthRaw(getGameState().getAction());
}

} // namespace refac

