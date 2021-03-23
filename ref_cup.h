// ref_cup.h
//
// Provide class "Cup" maintaining a consistent triple of "Throw", "Dice" and
// "Selection_t" objects.
//
// Throw:
//  - getters
// Dice:
//  - getters
//  - setters
// (Throw setters would be ambiguous.)
//
// The "Throw" part only represents the subset of *active* dice.


#ifndef REF_CUP_H_INCLUDED
#define REF_CUP_H_INCLUDED 1


#include <tuple> // std::pair

#include "ref_dice.h"
#include "ref_throw.h"
#include "ref_types.h"


namespace refac
{

class Cup
{
    Dice dice;          // All dice regardless of activeness
    Throw thrown;       // Only active dice
    Selection_t active; // Activeness mask for member 'dice'

  public:
    explicit Cup(Count_t dieCount, Digit_t initialDigit = 1);

    bool operator==(Cup const&) const; // Exact match of digits, activeness, and positions
    bool operator==(Throw const&) const; // Only active dice, disregarding permutations

    Count_t anyCount() const;
    Count_t activeCount(DigitType d = DigitType::total) const;
    Count_t inactiveCount() const;

    std::pair<Digit_t, bool> getDie(int pos) const;
    void setDie(int pos, Digit_t);              // Re-assign existing die digit
    void setDie(int pos, Digit_t, bool active); // Re-assign existing die digit + activeness
    void addDie(Digit_t, bool newActive = true);   // Add new die to the end
    void addDie(DigitType, bool newActive = true);
    void setActive(int pos, bool newActive = true);
    void toggleActive(int pos);
    Selection_t const& getActive() const;

    void roll(); // Randomize all active

    // copy + move ctor default
    // copy + move assign default
    // dtor default
};

} // namespace refac


#endif // REF_CUP_H_INCLUDED
