#include "tenthousand.h"

namespace from_rl_bases
{

/// ====================================================================
// TenKThrow implementation

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

/// ====================================================================
// TenKState implementation

bool TenKState::operator==(TenKState const& other) const
{
    return ( isTerminal() && other.isTerminal() )
        || ( _p == other._p && *_cup == *other._cup );
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

TenKState& TenKState::operator=(TenKState const& src)
{
    *_cup = *src._cup;
    _p = src._p;
    return *this;
}

TenKState& TenKState::operator=(TenKState&& src)
{
    if( _cup )
        delete _cup;
    _cup = src._cup; src._cup = 0;
    _p = src._p;
    return *this;
}

TenKState::TenKState()
  : _cup(new Throw_t()), _p(0)
{}

TenKState::TenKState(TenKState const& src)
  : _cup(new Throw_t(*src._cup)), _p(src._p)
{}

TenKState::TenKState(TenKState&& src)
  : _cup(src._cup), _p(src._p)
{
    src._cup = nullptr;
}

TenKState::TenKState(Throw_t const& t, Points_t const& p)
  : _cup(new Throw_t(t)), _p(p)
{}

TenKState::~TenKState()
{
  if( _cup != nullptr )
    delete _cup;
}

/// ====================================================================
// TenKMove implementation

} // namespace from_rl_bases

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
