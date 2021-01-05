// ui.h
//
// class Ui
//
// This class coordinates running games between multiple parties. Notably bots
// and players. It queries in- and outputs from players/bots and plays out
// their actions in the game environment.
//
// Display and Actors do not directly interact with the game class Tenthousand.
// Instead, the ui provides getters relaying to the environment.
//
// Display and Actors do not directly interact with the game class Tenthousand.
// Instead, the ui provides getters relaying to the environment. This is
// because we may want to provide more information than what the game class
// offers (e.g., using the history or any of the other actors). We may want to
// change this in the future.
// TODO since actors already act on the game itself it would onl be
//      consequential to do the same with display. ui can still provide
//      additional data through it's own getters. (?)

#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED 1


#include "actor.h"
#include "display.h"
#include "history.h"
#include "tenthousand.h"
#include "ui_types.h"


namespace ui
{

class Ui
{
  public:
    using Game_t = Tenthousand<3>;

    using History_t = History<Game_t::State_t>;
  private:

    friend class UiFactory;

    // TODO
    // We can do generic versions of this later. Lets focus on
    // getting something running first.

    // Owning all these objects
    Game_t*     game;
    BotActor*   bot;
    HumanActor* p1;
    HumanActor* p2;
    History_t*  history; // TODO
    Display*    display;

    Ui(); // Private. Construction is done by class UiFactory

    // Objekte um algorithmen auf die history annzuwenden
    // Statistics statistics1;

    void act(Game_t::Player);
    void act(); // Current player

  public:

    //__________________________________________________________________________
    // Internal interface

    // Get current "board" situation
    Game_t::Player getPlayer() const;
    Points_t getTurnPoints() const;
    bool getDieAside(int die) const;
    int getDieDigit(int die) const;
    bool canStopTurn() const;
    State_t getState() const;

    // Get global game situation
    Points_t getPoints(Player_t player) const;
    bool isOver() const;
    Game_t::Player getWinner() const;

    // Checks for human interaction:
    // islegal()
    // bringshowmanypoints()
    // ...

    // Relaying game interactions
    void finishTurn();
#ifdef RELAY
#error Redefinition of macro RELAY
#else
#define RELAY(f) \
    template<typename... Types> \
    auto f(Types... args) { return game-> f (args...); }
#endif
    RELAY(putAside)
    RELAY(roll)
    RELAY(finishTurn)
    RELAY(makeMove)
#undef RELAY

    // Print current situation in formatted way to stdout
    void rePrint();
    // Go back in history
    void rewind();

    //update buffers();

    // External Interface
    void startGame();

    // TODO free owned objects in dtor
    ~Ui();

    // TODO Rule of 3
    // TODO Rule of 5
};

// =============================================================================

// Class UiFactory collects the sub-component objects required for construction
// of a Ui object. New objects are created for missing components.
class UiFactory
{
  private:
    // TODO Simply derive privately from Ui?
    Ui* ui = new Ui{}; // Does not initialize ui in meaningfull way

    Game_t* game = nullptr;
    BotActor* bot = nullptr;
    HumanActor* p1 = nullptr;
    HumanActor* p2 = nullptr;
    Ui::History_t* history = nullptr;
    Display* display = nullptr;

//    Ui& getUi() { return ui; }

    // TODO Eventually, subcomponent type could provide a default, rather than
    //      the factory implementing a specific one.
    Game_t*        createDefaultGame();
    BotActor*      createDefaultBot();
    HumanActor*    createDefaultPlayer();
    Ui::History_t* createDefaultHistory();
    Display*       createDefaultDisplay();

    // Hand ownership of external subcomponent to ui.
    // TODO Use rrefs to communicate loss of ownership to caller (?)
    UiFactory& set(Game_t*);
    UiFactory& set(BotActor*);
    UiFactory& set(HumanActor*);  // Sets first not yet initialized HumanActor
    UiFactory& set(Ui::History_t*);
    UiFactory& set(Display*);

    UiFactory& setMissing();

    // Finalize Ui object owning the collected components.
    void create();

  public:
    template<typename T,
             typename = decltype(UiFactory::set(&std::declval<T>()))
            >
    UiFactory& operator<<(T* t);

    // Obtain ownership of the prepared Ui object
    operator Ui*() &&;
};

template<typename T>
void linkSubobjectToUi(T& subobject, Ui&);

} // namespace ui


#include "ui.inc"


#endif // UI_H_INCLUDED


// Erstmal ein paar gedanken was ein ui können sollte und wie das passieren könnte.

// Anforderungen
//
//  - Mehrere Spieler
//    - Mehrere Menschen
//    - Mehrere Bots
//  - Vollständige Spiel-Histories bereitstellen
//    - rewind nach missclick
//    - ggf live statistik erlauben
//    - speicherung von spielabläufen erlauben
//    - ggf offline statistik erlauben
//  - Schrittwiese eingabe für menschliche Spieler
//  - Schrittweise UND sofortige eingabe für bots
//    - schrittweise ist nicer visueller effekt
//  - KI selbst liefert nur fertige actions, UI schlüsselt in unter-aktionen auf
//  - Für die re-draw methode muss eine Art vollständige Spielbrett Darstellung verfügbar sein
//  - Quick-check nach den spielregeln möglicherweise, aber gefährlich falls wir sie ändern wollen
//    - ggf einfach ein warn-symbol
//    - soll versehentliche illegale Züge verhindern
//    - im Schadensfall rollback
//  - Actions buffern, sodass einzelne Würfel mit mehr Freiheit ausgewählt werden können.
//    - die Auswahl selbst ändert sich als Spielmechanik vermutlich nicht, darum recht sicher
//  - Auto-Vervollständigung von triplets
//    - Wenn, dann abschaltbar falls Regeländerung
//  - Modular-Mäßige erweiterungen
//    - einbinden von algorithmen welche aus der statistik durchschnitte usw. berechnen
//  - grafische elemente
//    - state (konkrete, ggf nicht markov'sche entscheidungssituation)
//    - full state (die aus meschnlicher sicht vollständige spielsituation)
//      - points from all players
//      - who's turn it is
//      - current turn-points
//      - whether or not player still needs to put sth aside
//      - hinweise zur (ill-)legalität der aktuell rausgelegten würfel
//        - ggf hat die Game_t klasse da bereits ein interface

// EinigeEinige Offene Fragen
//
//  - sollte der bot vorher oder währenddessen trainiert werden?
//  - wollen wir botts speichern und laden oder immer neu trainieren?
//  - soll das training eines passenden bots teil des UI sein?
//  ODER
//  - soll training separat vorgenommen und verwaötet werden und der fertige
//    bot dem ui zur Verfügung gestellt werden?
//  - haben wir irgendwelche effizienz ansprüche? Denke nicht.

// Umsetzung
//
//  - Bot interface
//    - Ref auf externen bot speichern und actions anfragen
//      ODER
//    - Intern bots erstellen und verwalten
//  - Darstellung
//    - ncurses ausgraben?
//    - cls + complett neu schreiben war nicht optimal,
//      aber ausreichend und am einfachsten
//  - bot interface vielleicht ähnlich wie statistik-erweiterungen?
//  - für die sequenzielle darstellung (mensch-simulation) von bots ein BOT-WRAPPER
//    - beutzt den bot um action zu erhalten
//    - abhängig von der action werden - wie bei einem mensch - tastendrücke simuliert
//      - ui interface sollte tastenbasierung nicht erfordern. dann kann es
//        auch für anderes testing verwendet werden
//    - wrapper kann reihenfolge von teil-aktionen randomisieren
//    - wrapper sollte austauchbar sein falls wir andere probieren wollen
//    - bot wrapepr kann textausgaben machen
//      - generische
//      - nach besonders guten/schlechten situationen
//      - "sprach-modul" sollte wohl wiederum separat realisiert werden

// Class Ui is currently primarily providing interfaces between each data
// member (display, actors, game, history, ...). This is so that only the Ui
// class must be adjusted if and of the other changes. not sure if that's worth
// it. Ui also owns all the objects.

// TODO Add means of explicit communication from UI modules to human/display.
//      Most easily: reserve one line of the display for each of the components
//      and allow them to set and clear them.

// Incorporation roadmap
//
// We want to:
// 1. create 1 ui object
// 2. the ui object creates and initializes the required sub-components (actors, history, dispaly)
// 3. we get the opportunity to configurate the ui to our needs
//    - select the bot level
//    - ( maybe in the future: player number, ruleset, ...)
//
// Open question: How do we watn to implement this?
// 1. Configuration class? That is like the HumanActor console interface, leading us through configuration in a config wirazd way?
// 2. Pass entire configuration to ctor, maybe in a configuration or settings struct?

// Other open questions:
//  1. Does the UI train the bot from scratch or do we provide an external bot?
//     Probably external is better to allow for load-store stuff and, for
//     example, executing the program in training mode where it continues
//     training a persistent bot intead of engaging in a game with the user
//  2. More general: What EXACTLY is the UI supposed to do?
