#include "tenthousand.h"

#include <cassert>

namespace from_rl_bases
{
void TenKThrow::checkInit() const
{  
  if( !isInit() )
    throw rl::UninitializedError("Tehnthousand throw not initialized.");
}

void TenKThrow::fill(size_t v)
{
  if( !isInit() )
    init();
  _arr->fill(v);
}

bool TenKThrow::operator==(TenKThrow const& other) const
{
  return
    _arr == other._arr
    ||
    (
     // (*_arr)[ALL] == (*other._arr)[] &&     
     // (implicit)
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
  assert(_arr != src._arr);

  if( _arr == nullptr )
    _arr = new DiceArray_t(*src._arr);
  else
    *_arr = *src._arr;
  return *this;
}

TenKThrow& TenKThrow::operator=(TenKThrow&& src)
{
  assert(_arr != src._arr);

  clear();
  _arr = src._arr;
  src._arr = nullptr;
  return *this;
}

TenKThrow::TenKThrow(DiceArray_t const& a)
  : _arr{ new DiceArray_t(a) }, _arrR(*_arr)
{}

TenKThrow::TenKThrow()
  : _arr( new DiceArray_t() ), _arrR(*_arr)
{
  _arr->fill(0);
}

TenKThrow::TenKThrow(TenKThrow const& src)
  : _arr( new DiceArray_t(*src._arr) ), _arrR(*_arr)
{}

TenKThrow::TenKThrow(TenKThrow&& src)
  : _arr(src._arr), _arrR(*_arr)
{
  src._arr = nullptr;
}

TenKState TenKState::randomStart()
{
  TenKState s;
  s.thrown()[6] = 6;
  for( unsigned int i = 0; i < 6; ++i )
    ++s.thrown()[rand() % 6];
  s.points() = 0;
  return s;
}

bool TenKState::operator==(TenKState const& other) const
{
  return ( isTerminal() && other.isTerminal() )
      || ( _p == other._p && Throw_t::operator==(other) );
}

//auto
//TenKState::operator=(TenKState const& src) noexcept
//  -> TenKState&
//{
//  if( this != &src )
//  {
//    Throw_t::operator=(src);
//    _p = src._p;
//  }
//  return *this;
//}

//auto
//TenKState::TenKState::operator=(TenKState&& src) noexcept
//  -> TenKState&
//{
//  if( this != &src )
//  {
//    Throw_t::operator=( std::move(src) );
//    _p = src._p;
//  }
//  return *this;
//}

//TenKState::TenKState(TenKState const& src)
//  :   rl::State(src), Throw_t(src), _p(src._p)
//{}

//TenKState::TenKState(TenKState&& src)
//  :   rl::State(src), Throw_t( std::move(src) ),
//      _p(src._p)
//{}

TenKState::TenKState(Throw_t const& t, Points_t const& p)
  : Throw_t(t), _p(p)
{}

TenKMove::TenKMove(Throw_t&& t, bool b)
  : rl::Action(), Throw_t( std::move(t) ), _finishes(b)
{}


} // namespace from_rl_bases

namespace tenthousand_states
{
    auto
    State::randomStart() -> State
    {
        State s;
        s.thrown()[6] = 6;
        for( unsigned int i = 0; i < 6; ++i )
            ++s.thrown()[rand() % 6];
        s.points() = 0;

        return s;
    }
}
