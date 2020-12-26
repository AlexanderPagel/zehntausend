#ifndef DICE_HPP_INCLUDED
#define DICE_HPP_INCLUDED


#include <cstdlib>
#include <array>
#include <stdexcept>
using namespace std;

template<unsigned int N>
class Dice
{
    array<unsigned int, N> dices = {{0}};

public:
    Dice() = default;
    explicit Dice( std::array<unsigned int, N> const& ) = delete;
    explicit Dice( std::array<unsigned int, N>&& src ) : dices( std::move(src) )
    {}

    bool operator==(Dice&& d) { return dices == d.dices; }

    Dice& roll(void);
    Dice& roll(std::array<bool,N> const& active);

    unsigned int operator[](size_t i) const;
};


// -------------------------------------------------------------------------
// implementation ----------------------------------------------------------

template<unsigned int N>
Dice<N>& Dice<N>::roll()
{
    for( auto i = dices.begin(); i != dices.end(); ++i)
        *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
Dice<N>& Dice<N>::roll(array<bool,N> const& active)
{
    auto j = active.begin();
    for( auto i = dices.begin(); i != dices.end(); ++i, ++j)
            if( *j ) *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
unsigned int Dice<N>::operator[](size_t i) const
{
    // array::operator[] does not bounce check so check before
    if( i >= N )
        throw out_of_range("There are not so many dice!");
    return dices[i];
}


#endif // DICE_HPP_INCLUDED
