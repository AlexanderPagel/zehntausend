// dice.h
//
// class Dice
//    - Basically an array of size N
//    - contains the digit shown by each die

#ifndef DICE_HPP_INCLUDED
#define DICE_HPP_INCLUDED 1


#include <cstdlib>
#include <array>
#include <stdexcept>

template<unsigned int N>
class Dice
{
    std::array<unsigned int, N> dice = {{0}};

public:
    Dice() = default;
    explicit Dice( std::array<unsigned int, N> const& src) : dice(src) {}
//    explicit Dice( std::array<unsigned int, N>&& src ) : dice( std::move(src) )
//    {}

    bool operator==(Dice const& other) { return dice == other.dice; }

    // Roll all or a subset of the dice
    Dice& roll();
    Dice& roll(std::array<bool,N> const& active);

    unsigned int operator[](size_t i) const;
};

#include "dice.inc"

#endif // DICE_HPP_INCLUDED
