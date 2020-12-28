#include "tenthousand.h"

namespace from_rl_bases
{


void TenKThrow::checkInit() const
{
  if( !isInit() )
    throw std::logic_error("TenKThrow not initialized");
}

void TenKThrow::fill(size_t v)
{
    if( !isInit() )
        init();
    _arr->fill(v);
}

bool TenKThrow::operator==(TenKThrow const& other) const
{
    return _arr == other._arr
        || ( // implicit
             // (*_arr)[ALL] == (*other._arr)[] &&
             (*_arr)[ONE]   == (*other._arr)[ONE] &&
             (*_arr)[TWO]   == (*other._arr)[TWO] &&
             (*_arr)[THREE] == (*other._arr)[THREE] &&
             (*_arr)[FOUR]  == (*other._arr)[FOUR] &&
             (*_arr)[FIVE]  == (*other._arr)[FIVE] &&
             (*_arr)[SIX]   == (*other._arr)[SIX]
           );
}

TenKThrow& TenKThrow::operator=(TenKThrow const& src)
{
    if( _arr == nullptr )
        _arr = new DiceArray_t(*src._arr);
    else *_arr = *src._arr;
    return *this;
}

TenKThrow& TenKThrow::operator=(TenKThrow&& src)
{
  clear();
  _arr = src._arr;
  src._arr = nullptr;
  return *this;
}

}

namespace tenthousand_states
{
//    auto
//    State::randomStart() -> State
//    {
//        State s;
//        s.thrown()[6] = 6;
//        for( unsigned int i = 0; i < 6; ++i )
//            ++s.thrown()[rand() % 6];
//        s.points() = 0;

//        return s;
//    }
}
