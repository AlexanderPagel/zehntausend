// ref_dice.h
//
// Class "Dice" represents a set of individual, ordered dice w/o extra
// information.


#ifndef REF_DICE_H_INCLUDED
#define REF_DICE_H_INCLUDED 1


#include <vector>

#include "ref_types.h"


namespace refac
{

class Dice
{
    // TODO Derive instead? size, empty, ...
    std::vector<Digit_t> dice; // digits in range [1, 6]

  public:
    explicit Dice(Count_t dieCount, Digit_t initialDigit = 1); // Instantiate c dice

    Digit_t  operator[](int pos) const;
    Digit_t& operator[](int pos);

    Count_t count() const;

    bool operator==(Dice const&) const; // TODO provide implementation

    void roll();
    void roll(Selection_t const& selection);

    void addDie(Digit_t);
};

} // namespace refac


#endif // REF_DICE_H_INCLUDED
