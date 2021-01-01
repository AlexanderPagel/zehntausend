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
    Ui const& ui;

    std::istream& is = std::cin;

  private:
    char getChar() const;

  public:
    // Use UI relay interfact to act on the game
    void operator()();

    HumanActor();
};

class BotActor
{
};

template<typename T>
bool isDieDigit(T const&);

} // namespace ui


#include "actor.inc"


#endif // ACTOR_H_INCLUDED
