#include "ref_tenthousand.h"


namespace refac
{

void
Tenthousand::addPoints(Points_t points)
{
  assert(points > 0);
  savePoints[player] += points;
}

void
Tenthousand::restartEnvironment()
{
  gameStates[player].restart();
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
template<GameState::* Member, typename... Types>
bool interact(Types... args)
{
  assert(!hasFinished());

  // Class "GameState" ensures that game interactions are nops when failing.
  // Then, cleanup is unnecessary since the game states are just like before.
  auto success{(getGameState().*Member)(args...)};
  if(success == false)
    return false;

  // If episode was finished, we need some cleanups
  if (getGameState().isTerminal())
  {
    auto const& gs = getGameState();
    addPoints(gs.getReturn());
    gs.restart();
    adjustWinner();
    incrementPlayer(); // Must be last because is changes current player
  }

  return true;
}

Tenthousand::Tenthousand(int playerCount = 0)
  : gameStates(playerCount),
    savePoints(playerCount, 0)
{}

int
Tenthousand::playerCount() const
{
  return static_cast<int>(gameStates.size());
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
Tenthousand::winner() const
{
  assert(hasFinished());
  return winner;
}

GameState const&
Tenthousand::gameState() const
{
  return gameStates[player];
}

State const&
Tenthousand::getState() const
{
  return gameState().getState();
}

Cup const&
Tenthousand::getCup() const
{
  return getState().getCup();
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
  return interact(&GameState::toggleAside, pos);
}

bool
Tenthousand::interactRoll()
{
  return interact(&GameState::roll);
}

bool
Tenthousand::interactFinish()
{
  return interact(&GameState:finish);
}

} // namespace refac

