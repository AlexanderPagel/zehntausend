#include "ui.h"

#include <string> // std::to_string


namespace ui
{

Ui::Ui()
  : game(nullptr)
  , bot(nullptr)
  , p1(nullptr)
  , p2(nullptr)
  , history(nullptr)
  , display(nullptr)
  , average(nullptr)
  , rollback(nullptr)
{
  // No meaningfull initialization by this ctor.
  // Class UiFactory initializes Ui objects.
}

void
Ui::act(Game_t::Player player)
{
  switch (player)
  {
    case 0:
      (*bot)();
      break;
    case 1:
      (*p1)();
      break;
    case 2:
      (*p2)();
      break;
    default:
      assert(false && "unreachable");
  };
  return;
}

void
Ui::act()
{
  // Keep track of points to update average
  auto playerToMove{getPlayer()};
  auto pointsBefore{getPoints(playerToMove)};

  act(playerToMove);

  // TODO This breaks when there is only 1 actor/player
  // If player is no longer to move, update average for her
  if (getPlayer() != playerToMove)
  {
    auto pointsAfter{getPoints(playerToMove)};
    addToAverage(playerToMove, pointsAfter - pointsBefore);
  }
}

Game_t::Player
Ui::getPlayer() const
{
  // TODO assert(not terminal) once gmae class has such a state
  return game->getPlayer();
}

Points_t
Ui::getTurnPoints() const
{
  return game->getCurrent();
}

bool
Ui::getDieAside(int die) const
{
  // TODO In the game class i currently named this "active" instead of "aside".
  //      Find one name pls :)
  assert(0 <= die and die < 6);

  // If it is not active, it must be put aside
  return !game->getActive()[die];
}

int
Ui::getDieDigit(int idx) const
{
  assert(0 <= idx && idx <= 6);

  return game->_cup[idx];
}

bool
Ui::canStopTurn() const
{
  return game->getPutAny();
}

State_t
Ui::getState() const
{
  return game->state();
}

Points_t
Ui::getPoints(Player_t player) const
{
  assert(int(player) < 3);
  return game->getPoints(player);
}

bool
Ui::isOver() const
{
  // TODO
  return false;
}

Game_t::Player
Ui::getWinner() const
{
  // TODO
  return 0;
}

void
Ui::finishTurn()
{
  try
  {
    game->finishTurn();
  }
  // TODO If we remake the game class we probably want regular return
  //      values. the player not getting points is not an error in
  //      the program. The return value can then also be passed on
  //      normally to the actor (or other callers).
  catch (illegal_move_error& err) // Turn has also finished
  {
    // TODO Here we update internal states if we wish

    // TODO Maybe we want to re--throw to inform the bot about the error
  }
}

double
Ui::getAverage(Player_t const& player) const
{
  return average->getAverage(player);
}

void
Ui::addToAverage(Player_t const& player, Points_t const& turnPoints)
{
  average->addToAverage(player, turnPoints);
}

void
Ui::save()
{
  rollback->save(*game);
}

void
Ui::restore()
{
  rollback->restore(*game);
}

//Ui::Game_t const&
//Ui::getGame() const
//{
//  return *game;
//}

//void
//Ui::setGame(Game_t const& g)
//{
//  game = g;
//}

void
Ui::rePrint()
{
  display->draw();
}

// Ui objects are crated by the UiFactory class, thus all subobjects will be
// correctly constructed at any time.
void
Ui::startGame()
{
  // "Game loop"
  while (!isOver())
  {
    std::cout << "Starting new round." << std::endl;
    rePrint();
    act();  // Current player makes 1 game input
    std::cout << "Round is over." << std::endl;
  };

  std::cout << "Game has finished." << std::endl;
}

Ui::~Ui()
{
  delete game;
  delete bot;
  delete p1;
  delete p2;
  delete history;
  delete display;
  delete average;
}

// =============================================================================

Game_t*
UiFactory::createDefaultGame()
{
  return new std::remove_reference_t<decltype(*game)>{};
}

BotActor*
UiFactory::createDefaultBot()
{
  // FIXME Sarsa bot creation is currently a memory leak: Bot actor will not
  //       free bot.
  // TODO Within the ui namespace it would be simplest to use smart pointers
  //      and not care who owns what.
  //      Alternatively we can also come up with a suitable set of rules about
  //      who creates and owns objects.

  // Get learning parameters interactively from std::cin
  unsigned int count;
  double alpha;
  double epsilon;
  std::cout << "Legacy:" << std::endl
            << " steps                 | learning rate | exploration rate" << std::endl
            << "       0  -    999     | 0.25          | 0.2" << std::endl
            << "    1000  -   9999     | 0.18          | 0.15" << std::endl
            << "   10000  -  99999     | 0.14          | 0.12" << std::endl
            << "  100000+              | 0.12          | 0.1" << std::endl;
  std::cout << "Level (> 0): ";
  std::cin >> count; std::cin.sync(); std::cin.clear();
  std::cout << "Learning rate (0 to 1): ";
  std::cin >> alpha; std::cin.sync(); std::cin.clear();
  std::cout << "Exploration (0 to 1): ";
  std::cin >> epsilon; std::cin.sync(); std::cin.clear();
  std::stringstream ss;
  if (alpha <= 0 || 1 <= alpha)
    throw std::runtime_error("Alpha " + std::to_string(alpha) + " invalid");
  if (epsilon <= 0 || 1 <= epsilon)
    throw std::runtime_error("Epsilon " + std::to_string(epsilon) + " invalid");

  // Perform learning episodes
  std::cout << "Running " << count << " episodes with learning rate " << alpha
    << " and exploration " << epsilon << "..." << std::endl;
  Sarsa* b = new Sarsa(alpha, epsilon);
  b->performLearningEpisodes(count);
  std::cout << "Training finished. [ENTER]" << std::endl;
  std::cin.ignore();

  return new BotActor{ui, *b};
}

HumanActor*
UiFactory::createDefaultPlayer()
{
  return new HumanActor{ui};
}

Ui::History_t*
UiFactory::createDefaultHistory()
{
  return new History<State_t>{ui};
}

Display*
UiFactory::createDefaultDisplay()
{
  return new Display{ui};
}

Average<>*
UiFactory::createDefaultAverage()
{
  // Construct ui::Average object for 3 players
  return new Average{ui, 3};
}

Rollback<Game_t>*
UiFactory::createDefaultRollback()
{
  // Construct ui::Rollback object for type Ui::Game_t
  return new Rollback<Game_t>{ui};
}


UiFactory&
UiFactory::set(Game_t* g)
{
  assert(game = nullptr);
  this->game = g;
  return *this;
}

UiFactory&
UiFactory::set(BotActor* b)
{
  assert(bot == nullptr);
  bot = b;
  return *this;
}

UiFactory&
UiFactory::set(HumanActor* h)
{
  assert(p1 == nullptr || p2 == nullptr);
  if (p1 == nullptr)
    p1 = h;
  else
    p2 = h;
  return *this;
}

UiFactory&
UiFactory::set(Ui::History_t* h)
{
  assert(history == nullptr);
  history = h;
  return *this;
}

UiFactory&
UiFactory::set(Display* d)
{
  assert(display == nullptr);
  display = d;
  return *this;
}

UiFactory&
UiFactory::set(Average<>* a)
{
  assert(average == nullptr);
  average = a;
  return *this;
}

UiFactory&
UiFactory::setMissing()
{
  if (game     == nullptr) game     = createDefaultGame();
  if (bot      == nullptr) bot      = createDefaultBot();
  if (p1       == nullptr) p1       = createDefaultPlayer();
  if (p2       == nullptr) p2       = createDefaultPlayer();
  if (history  == nullptr) history  = createDefaultHistory();
  if (display  == nullptr) display  = createDefaultDisplay();
  if (average  == nullptr) average  = createDefaultAverage();
  if (rollback == nullptr) rollback = createDefaultRollback();

  return *this;
}

void
UiFactory::create()
{
  // Create default objects if none provided
  setMissing();

  // The default ctor of class Ui does not perform meaningfull initialization.
  // Set all non-static data members.
  ui->game = game;
  ui->bot = bot;
  ui->p1 = p1;
  ui->p2 = p2;
  ui->history = history;
  ui->display = display;
  ui->average = average;
  ui->rollback = rollback;
}

// This member function is callable for rvalues only because the caller should
// be aware that receiving the ui object invalidates the factory object.
UiFactory::operator Ui*() &&
{
  create();

  return ui;
}

} // namespace ui


// Factory roadmap
//  Yep thrown it over board. Lets construct Ui object first and
//  insert sub-objects in-place by factory.
