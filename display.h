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
     |      0  -> [3] [3] [3] [6] [1] [5]            1 line with rolled dice,
                                                                 currernt points,
                                                                 active indivcators, and
                                                                 yet-to-move indicators
*/


class Display
{
  private:
    // The Ui object where the shown data orginates
    Ui const& ui;
    std::ostream& os;

  private:
    // Computation of sub-expressions

    // Formatted sub-prints
    void drawPlayer(Ui::Game_t::Player p);

  public:
    // The interface that is called by our Ui class
    void draw();

    // TODO Set default os to std::cout
    Display();
};

// Draw one player line
void drawPlayerLine(
    std::ostream&,
    Ui::Game_t::Player const&,
    int points);

} // namespace ui


#endif // DISPLAY_H_INCLUDED
