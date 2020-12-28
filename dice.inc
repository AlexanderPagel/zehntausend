// dice.inc
//
// Implementation of template class Dice<>

#include <cassert>

template<unsigned int N>
Dice<N>& Dice<N>::roll()
{
    for( auto i = dice.begin(); i != dice.end(); ++i)
        *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
Dice<N>& Dice<N>::roll(std::array<bool,N> const& active)
{
    auto j = active.begin();
    for( auto i = dice.begin(); i != dice.end(); ++i, ++j)
            if( *j ) *i = (rand() % 6);
    return *this;
}

template<unsigned int N>
unsigned int Dice<N>::operator[](size_t i) const
{
    assert(i < N);

    // TODO there should be a separate function "at()" to take user input and perform bounds check
//    if( i >= N )
//        throw std::out_of_range("There are not so many dice!");

    return dice[i];
}
