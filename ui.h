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
// TODO since actors already acto on the game itself it would onl be consequential to do the same with display. ui can till provide additional data through it's own getters

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

    // We can do generic versions of this later. Lets focus on
    // getting something running first.
    Game_t     game;  // TODO Have our own or work on reference?
    BotActor   bot;
    HumanActor p1;
    HumanActor p2;
    History_t  history; // ?
    Display    display; // :)

    // Objekte um algorithmen auf die history annzuwenden
    // Statistics statistics1;

  public:
    // Get current "board" situation
    Game_t::Player getPlayer() const;
    Points_t getTurnPoints() const;
    bool getDieAside(int die) const;
    int getDieDigit(int die) const;
    bool canStopTurn() const;

    // Get global game situation
    Points_t getPoints(Player_t player) const;
    bool isOver() const;
    Game_t::Player getWinner() const;

    // Checks for human interaction:
    // islegal()
    // bringshowmanypoints()
    // ...

    // Relaying game interactions
#ifdef RELAY
#error Redefinition of macro RELAY
#else
#define RELAY(f) \
    template<typename... Types> \
    auto f(Types... args) { return game. f (args...); }
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
};



} // namespace ui


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
