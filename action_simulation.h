// action_simulation.h
//
// Class ActionSiumulation to translate an action into a corresponding sequence
// of button presses.
//
// We can imagine a button sequence being a graph-like, recursive structure in
// the future. For now, we just store a string of sequentially executed button
// presses.


#ifndef ACTION_SIMULATION_H_INCLUDED
#define ACTION_SIMULATION_H_INCLUDED 1


#include "ui_types.h"


namespace ui
{

class ActionSimulation
{
    std::string sequence;

    // Append key press to put aside die at position pos
    void addKeyPressForPos(int pos); // pos 0-based, key press 1-based
    void addKeyPressForEnd(bool roll); // End turn by pressing r or f

    void inferPressesToPutAside(Ui const&, Action_t const&);
    void inferPressesToCompleteTurn(Action_t const&);

  public:

    ActionSimulation(Ui const& ui, Action_t const&);
};

} // namespace ui


#endif // ACTION_SIMULATION_H_INCLUDED
