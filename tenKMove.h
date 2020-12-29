#ifndef TENKMOVE_H_INCLUDED
#define TENKMOVE_H_INCLUDED 1

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "dice.h"
#include "tenKThrow.h"

/// ====================================================================
///
class TenKMove
{
public:
    using Throw_t = TenKThrow;

private:
    Throw_t* _put;
    bool _finishes;

public:
    /// ----------------------------------------------------------------
    /// action requirements

        // empty

    /// ----------------------------------------------------------------
    /// game functionality
    Throw_t& putAside() { return *_put; }
    Throw_t const& putAside() const { return *_put; }
    bool& finishes() { return _finishes; }
    bool finishes() const { return _finishes; }

    static TenKMove zero() { return TenKMove(); }

    /// ----------------------------------------------------------------
    /// assignments
    auto
    operator=(TenKMove const& src)
        -> TenKMove&
    {
        if( this == &src ) return *this;

        *_put = *src._put;
        _finishes = src._finishes;
    }

    auto
    operator=(TenKMove&& src)
        -> TenKMove&
    {
        if( this == &src ) return *this;
        delete _put;
        _put = src._put; src._put = nullptr;
        _finishes = src._finishes;
    }

    /// ----------------------------------------------------------------
    /// ctors
    TenKMove() : _put(new Throw_t()), _finishes(false) {}
    TenKMove(TenKMove const& src) : _put(new Throw_t(*src._put)), _finishes(src._finishes) {}
    TenKMove(TenKMove&& src) : _put(src._put), _finishes(src._finishes) { src._put = nullptr; }

    TenKMove(Throw_t&& t, bool b) : _put(new Throw_t(std::move(t))), _finishes(b) {}
    ~TenKMove() { if( _put != nullptr ) delete _put; }
};

#endif