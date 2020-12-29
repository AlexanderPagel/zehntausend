#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED 1

#include "tenKThrow.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>


    class Move : private std::pair<TenKThrow,bool>
    {
        typedef TenKThrow Throw_t;
        typedef typename std::pair<Throw_t,bool> _base_t;
    public:
        Throw_t&       putAside()       { return _base_t::first; }
        Throw_t const& putAside() const { return _base_t::first; }
        bool&       finishes()       { return _base_t::second; }
        bool const& finishes() const {return _base_t::second; }

        static Move zero() { return Move( Throw_t{{0,0,0,0,0,0,0}},false ); }

        Move& operator=(Move const&) = default;

        Move(Move const& src) : _base_t( src ){}
        Move(Move&& src) : _base_t( src ) {}
        Move(Throw_t&& t, bool b) : _base_t( std::move(t), b) {}
    };


#endif