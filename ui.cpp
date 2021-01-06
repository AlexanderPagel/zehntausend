#include "ui.h"

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
    auto pointsAfter{getPoints(getPlayer())};
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
  //      Alternatively we can also come up with a suitable set of rule about
  //      who creates and owns objects.
  Sarsa* b = new Sarsa();

  std::cout << "Begin training default bot [ENTER]" << std::endl;
  std::cin.ignore();
  b->performLearningEpisodes(10000);
  std::cout << "Finished training default bot [ENTER]" << std::endl;
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
  if (game    == nullptr) game    = createDefaultGame();
  if (bot     == nullptr) bot     = createDefaultBot();
  if (p1      == nullptr) p1      = createDefaultPlayer();
  if (p2      == nullptr) p2      = createDefaultPlayer();
  if (history == nullptr) history = createDefaultHistory();
  if (display == nullptr) display = createDefaultDisplay();
  if (average == nullptr) average = createDefaultAverage();

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
