// actor.h
//
// Actor classes that interact with a given environment.
//
// Class HumanActor
//  Internally queries human input over stdin. Note that this actor class does
//  in no way communicate information to the player. All relevant information
//  must be presented to the user otherwise (using the display class).
//  The HumanActor class merely relays the human's decisions via console reads.
//
// Class BotActor
//  Internally holds a RL bot trained to play the game. Contrary to the human
//  version, this class does NOT simply relay bot decisions. Instead, based on
//  the bot's chosen actions, it simulates human like button presses. For
//  example, it may put dice asie one by one and then finish.
//  This is to allow the human observer to easily follow the bot agent's play
//  - with multiple display refreshes in-between.
//
// Actors
//  - validate inputs + correct missclicks
//  - give action to UI class which relays to game object


#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED 1


#include <iostream>

#include "ui_types.h"
#include "sarsa.h"


namespace ui
{

// Möglicherweise gedachtes Interface
class Actor
{
  public:
    // How to interact and what to interact with?
    void interact();  // Interact with ui?
};

class Ui;

class HumanActor
{
    // Required for interaction
    Ui& ui;

    // Required for implementation. 0 means everything normal.
    char lastInput = 0; // Do not block questionable input if given twice in a row
    void clearLast() { lastInput = 0; }
    bool hasLast() const { return lastInput != 0; }

    // Functions that determine the appropriate answer towards the UI after
    // use input is known. These assume valid user character input.
    void respondToDieInput(char);
    void respondToAll     (char);
    void respondToRoll    (char);
    void respondToFinish  (char);
    void respondToQuit    (char);
    // void respondToRewind();
    bool respondToSuspect (char); // Input sus. Returns true if confirmed.
    void respondToInvalid (char); // Input is not mapped to functionality

    // Check user input
    using RespondFunc_t = void (HumanActor::*)(char);
    RespondFunc_t classifyInput(char);     // Blocks questionable input the first time

  public:
    // Use UI relay interface to act on the game
    void operator()();      // Query console for input
    void operator()(char);  // Simulate key input

    HumanActor(Ui&);
};

class BotActor
{
  public:
    using Bot_t = Sarsa;

  private:
    Ui& ui;

    Bot_t const& bot;

    // Simulate button presses according to given action
    void respondWithKeyPresses(Action_t const&);

  public:
    // Use UI relay interface to act on the game
    void operator()();

    BotActor(Ui&, Bot_t const&);
};

template<typename T>
bool isDieDigit(T const&);

} // namespace ui


#include "actor.inc"


#endif // ACTOR_H_INCLUDED

// TODO We could make it such that capital letters are forced actions (ignoring
//      sanitizers).

// Roadmap HumanActor
// 1. get human input
// 2. check validity
// 3. Allow corrections if invalid
// 4. hand to UI class

// Roadmap BotActor
// 0. Load bot from somewhere
// 1. Query state from game object
// 2. Use bot to lookup action
// 3. Maybe add textual comment or action value to UI/Display
// 4. Simulate Human-Like key pressing according to bot action
//    (Hand action to Ui class in sub-actions)

// TODO The respondTOSuspect might not be the best mechanic.
//      Maybe simple re-getCh() explicitly and compare. That looping is very
//      difficult to follow.

// TODO Resonse function currently only handle the game aspect. Later we also
//      want them to pass something like a message code or string to the
//      UI/display as interactive feedback.
// TODO Maybe the display class can reserve specific areas for string messages
//      to the user by any of the other UI components. Ui class needs to
//      provide approprite interface to comunicate w/ the display class.

// TODO Maybe the ui classes should communicate directly to each other?
