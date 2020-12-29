#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>


#include "tenKThrow.h"


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

