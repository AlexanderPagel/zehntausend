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
{
  // No meaningfull initialization by this ctor.
  // Class UiFactory initializes Ui objects.
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
Ui::getDieAside(int) const
{
  // TODO Not implemented in game class or need to go over state
  assert(false);
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
UiFactory::setMissing()
{
  if (game    == nullptr) game    = createDefaultGame();
  if (bot     == nullptr) bot     = createDefaultBot();
  if (p1      == nullptr) p1      = createDefaultPlayer();
  if (p2      == nullptr) p2      = createDefaultPlayer();
  if (history == nullptr) history = createDefaultHistory();
  if (display == nullptr) display = createDefaultDisplay();

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
