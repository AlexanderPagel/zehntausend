// dice.inc
//
// Implementation of template class Dice<>

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
    // array::operator[] does not bounce check so check before
    if( i >= N )
        throw std::out_of_range("There are not so many dice!");
    return dice[i];
}
