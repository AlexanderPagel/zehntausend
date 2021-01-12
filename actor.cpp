#include "actor.h"

#include <iostream> // dbg
#include <string>

#include "console.h"
#include "ui.h"
#include "action_simulation.h"

namespace ui
{

void
HumanActor::respondToDieInput(char input)
{
  getUi()->save();
  // TODO
  // Currently, we can either sanity check inputs directly or throw them at
  // the game class and wait for an error.
  // In the long run, we want a non-sanitized base game class and another,
  // explcitly sanitized wrapper class.
  // The sanitizer class could probably already provide simple 1-turn
  // rollbacks.
  // Could be called class SafeGame where it is simply impossible to make
  // illegal non-revertable decisions.

  // TODO In the long term, the game class should allow re-activation of dice
  // previously put aside. Not currently possible.

  // Type "1" to reference the 1st die, "6" for the last (6th)
  getUi()->putAside(input - '1');
  // FIXME We need to catch in case die cannot be put aside,
  //       or prevent such cases.
}

void
HumanActor::respondToAll(char)
{
  getUi()->save();
  getUi()->putAside();
}

void
HumanActor::respondToRoll(char)
{
  getUi()->save();
  getUi()->roll();
}

void
HumanActor::respondToFinish(char c)
{
  getUi()->save();
  // TODO Identify whether or not there are still free points available and all
  //      respondToSuspect if true (instead of calling respondToAll())).
  //      Usesage:  if (respondToSuspect(input))
  //                  // confirmed

  // Sanitize: Put everything aside when finishing.
  respondToAll(c);  // Argument is irrelevant

  getUi()->finishTurn();
}

void
HumanActor::respondToRestore(char)
{
  getUi()->restore();
}

void
HumanActor::respondToQuit(char)
{
  assert(false);
  // TODO What should even happen here?
}

bool
HumanActor::respondToSuspect(char input)
{
  // First time we reach this func we ignore and try again
  if (!hasLast())
  {
    // Save input that needs confirmation
    lastInput = input;
    operator()(); // Retry recursively
    return false;
  }

  // Second time we reach this func we do it if input is the same
  else
  {
    return input == lastInput;
  }
}

void
HumanActor::respondToInvalid(char input)
{
  if (respondToSuspect(input))
  {
    // TODO Issue message for player towards Ui/Display message
  }
}

HumanActor::RespondFunc_t
HumanActor::classifyInput(char input)
{
  // Interface keys

  // (dice #)
  if ('1' <= input && input <= '6') return &HumanActor::respondToDieInput;
  // (All)
  if (input == 'a')                 return &HumanActor::respondToAll;
  // (Roll)
  if (input == 'r')                 return &HumanActor::respondToRoll;
  // (Finish)
  if (input == 'f')                 return &HumanActor::respondToFinish;
  // (Undo)
  if (input == 'u')                 return &HumanActor::respondToRestore;
  // (get Lost)
  if (input == 'l')                 return &HumanActor::respondToQuit;

  // Unmapped keys
  return &HumanActor::respondToInvalid;
}

void
HumanActor::operator()()
{
  // Get 1 character of user input from console
  char input{console::getChar()};
  return operator()(input);
}

void
HumanActor::operator()(char input)
{
  // Handle user input
  auto&& responseFunction{classifyInput(input)};
  (this->*responseFunction)(input);

  // lastInput is set internally when a confirmation is needed from recursion
  clearLast();
}

// =============================================================================

void
BotActor::respondWithSimulatedKeyPresses(Action_t const& action)
{
  // Translate abstract action into concrete key inputs using Ui state
  // interface to infer required button presses.
  ActionSimulation const simulatedKeys(*getUi(), action);

  // TODO Eventually, we may here put our HumanActor sub-object into strict
  //      mode (omitting confirmation requests). So that the bot is able to
  //      perform obviously poor actions undistirbed.

  // Pretend to be a human actor pressing these keys in order
  for (auto const& k : simulatedKeys)
    this->HumanActor::operator()(k);
}

void
BotActor::operator()()
{
  auto state{getUi()->getState()};

  auto action{bot.greedy(state)};  // TODO Define operator() as agent interface

  // TODO Add textual coment to display via some means

  respondWithSimulatedKeyPresses(action);
}

BotActor::BotActor(Ui* ui, Bot_t const& bot)
  : HumanActor{ui}, bot(bot)
{}

} // namespace ui
