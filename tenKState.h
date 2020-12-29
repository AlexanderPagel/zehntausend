#ifndef TENKSTATE_H_INCLUDED
#define TENKSTATE_H_INCLUDED 1

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "dice.h"
#include "tenKThrow.h"
#include "Afterstate.h"




class TenKState
{
public:
    using Points_t = unsigned int;
    using Throw_t = TenKThrow;

private:
    Throw_t* _cup;  // TODO Throw already contains pointer
    Points_t _p;

public:
    bool isTerminal() const{ return !_cup->any(); }
    auto clone() const noexcept(true) { return new TenKState(*this); }
    bool operator==(TenKState const& other) const;

    Throw_t&       thrown() { return *_cup; }
    Throw_t const& thrown() const { return *_cup; }
    Points_t&       points() {return _p; }
    Points_t const& points() const { return _p; }
//    bool terminal() const { return isTerminal(); }

    static TenKState randomStart();
    TenKState(Throw_t const& t, Points_t const& p);

    TenKState& operator=(TenKState const& src);
    TenKState& operator=(TenKState&& src);

    TenKState();
    TenKState(TenKState const& src);
    TenKState(TenKState&& src);
    ~TenKState();
};


namespace std
{

template<> struct hash<TenKState>
{
    size_t operator()(TenKState const& x) const
    {
        return std::hash<unsigned int>()
        (
            x.isTerminal()
            ? (1 << (sizeof(unsigned int)*8 -1))
            : ( x.thrown()[0] << 0  |
                x.thrown()[1] << 3  | // max size is 8 ^= 3 bit
                x.thrown()[2] << 6  |
                x.thrown()[3] << 9  |
                x.thrown()[5] << 12 |
                x.thrown()[4] << 15 |
                x.thrown()[6] << 18 |
                x.points()    << 21)
         );
    }
};

template <> struct hash<Afterstate>
{
    size_t operator()(Afterstate const& x) const
    {
        return std::hash<unsigned int>()
        (
            x.fromTerminal() // 0/0
            ? (1 << (sizeof(unsigned int)*8 -1))
            : ( x.diceLeft()     <<   0 |
                x.points()       <<   3 |
                x.pointsBefore() <<  15) // reserving 12 bit for < 4096 points
        );
    }
};

} // namespace std

#endif