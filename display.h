// display.h
//
// Declare class Display to print an entire game state (to stdout).
//
// While the class Ui handles useer and bot inouts/outputs, this class is
// solely resposible for laying out the game state in a formatted, structured
// way on a console/textfile.
//
// We seek to display game states in snapshots. This class implements the
// design of such a snapshot.

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED 1


#include <ostream>

#include "ui_types.h"
#include "ui_subobject.h"


namespace ui
{

// Preliminary screen setup:
/*
Player 1:    50                                      3 player lines
Player 2:   300                                      .
Player 3: 10000                                      .
                                                     1 empty line
     |    Dice:    1   2   3   4   5   6             1 never changing default line
   0 |                                               1 line just with player number
     |    _150 -> [3] [3] [3] [6] [1] [5]            1 line with rolled dice,
                                                                 currernt points,
                                                                 active indivcators, and
                                                                 yet-to-move indicators
*/

class Display : public UiSubobject
{
  private:
    std::ostream& os = std::cout; // TODO really needed?

  public:
    // Display layout parameters
    static constexpr int pointsWidth = 5;
    static constexpr int currentPointsWidth = 4;
    static constexpr int playerCount = 3;

  private:
    // Need to start at 0 to be used as indices
    enum class ConstLineIdx { emptyLine, diceNumLine };

    void clear();

    // Computation of sub-expressions
    char dieChar(int die) const;  // Character representing die at index "die"

    // Formatted sub-prints
    void drawPlayerLine(Game_t::Player p);
    void drawConstantLine(ConstLineIdx);
    void drawPlayerToMoveLine();
    void drawCurrentThrowLine();

  public:
    // The interface that is called by our Ui class
    void draw();

    using UiSubobject::UiSubobject;
};

} // namespace ui


#endif // DISPLAY_H_INCLUDED
