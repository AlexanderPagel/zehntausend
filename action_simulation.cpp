#include "action_simulation.h"

#include "ref_types.h"
#include "ui.h"
#include "ui_types.h"

namespace ui
{

void
ActionSimulation::addKeyPressForPos(int pos)
{
  assert(0 <= pos && pos < 6);
  sequence.push_back('1' + pos);
}

void
ActionSimulation::addKeyPressForEnd(bool roll)
{
  sequence.push_back(roll ? 'r' : 'f');
}

void
ActionSimulation::inferPressesToPutAside(Ui const& ui, Action_t const& action)
{
  // Loops through all the digits that need to be put aside. For each, search
  // the set of {thrown dice not put aside} for a matching die and put it
  // aside.

  // "digit" == the digit we investigate now
  // "n" == number of times a die showing "digit" need to be put aside to fulfill action
  // "pos" == index where we search for a die showing "digit" to be put aside

  for (auto digit = refac::DigitType::one; digit <= refac::DigitType::six; ++digit)
    if (auto n = action.taking[digit]; n > 0)
    {
      // Test each possible position to find corresponding die
      for (int pos = 0; pos < 6; ++pos)
        if (!ui.getDieAside(pos) && ui.getDieDigit(pos) == digitTypeToDigit(digit))
        {
          addKeyPressForPos(pos); --n;
          if (n == 0) break;
        }

      // For legal actions this does not trigger because each digit
      // to be put aside is found in game state.
      assert(n == 0);
    }

  // TODO Probly faster:
  // Alternative implementation: looop through rolled dice and put each aside
  // if corresponding value in action is present.
}

void
ActionSimulation::inferPressesToCompleteTurn(Action_t const& action)
{
  // If action does not finish, add key press for rolling, else for finish
  addKeyPressForEnd(!action.finish);
}

ActionSimulation::ActionSimulation(Ui const& ui, Action_t const& action)
{
  assert(!action.isNone());

  // TODO This is checking for the zero action. Probably need a member for this in the action type
  if (action.taking[refac::DigitType::total] == 0 && action.finish == false)
  {
    assert(false && "None actions should not happen, ever, in the refactoring");
    // Map zero action to simply finishing immediately
    addKeyPressForEnd(false);
    return;
  }

  // For normal actions, split in put aside phase and ending phase
  inferPressesToPutAside(ui, action);
  inferPressesToCompleteTurn(action);
}

} // namespace ui
