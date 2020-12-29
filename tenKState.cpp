
#include "tenKState.h"

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