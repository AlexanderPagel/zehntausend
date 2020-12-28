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
using namespace std;

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
    Dice& roll(void);
    Dice& roll(std::array<bool,N> const& active);

    unsigned int operator[](size_t i) const;
};


// -------------------------------------------------------------------------
// implementation ----------------------------------------------------------

template<unsigned int N>
Dice<N>& Dice<N>::roll()
{
    for( auto i = dice.begin(); i != dice.end(); ++i)
        *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
Dice<N>& Dice<N>::roll(array<bool,N> const& active)
{
    auto j = active.begin();
    for( auto i = dice.begin(); i != dice.end(); ++i, ++j)
            if( *j ) *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
unsigned int Dice<N>::operator[](size_t i) const
{
    // array::operator[] does not bounce check so check before
    if( i >= N )
        throw out_of_range("There are not so many dice!");
    return dice[i];
}


#endif // DICE_HPP_INCLUDED
