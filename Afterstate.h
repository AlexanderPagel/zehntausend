#ifndef AFTERSTATE_H_INCLUDED
#define AFTERSTATE_H_INCLUDED 1

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>

    class Afterstate : private std::pair<unsigned int,unsigned int>
    {
        typedef typename std::pair<unsigned int,unsigned int> _base_t;
        bool _fromTerminal = false;
        unsigned int _pointsBefore = 0;
    public:
        bool&       fromTerminal()       { return _fromTerminal; }
        inline bool fromTerminal() const { return _fromTerminal; }
        unsigned int& diceLeft()       { return this->first; }
        unsigned int  diceLeft() const { return this->first; }
        unsigned int& points()        { return this->second; }
        unsigned int  points() const  { return this->second; }
        unsigned int& pointsBefore()       { return this->_pointsBefore; }
        unsigned int  pointsBefore() const { return this->_pointsBefore; }


        bool operator==(Afterstate const& other) const
        {
            return (fromTerminal() && other.fromTerminal())    // terminal states are the same
                || (_base_t::first == other._base_t::first &&
                    _base_t::second == other._base_t::second &&
                    _pointsBefore == other._pointsBefore);
        }

        Afterstate() = default;
        Afterstate(Afterstate const& src) : _base_t( src ),
                                            _fromTerminal(src._fromTerminal),
                                            _pointsBefore(src._pointsBefore) {}
        Afterstate(Afterstate&& src) :  _base_t( std::move(src) ),
                                        _fromTerminal(src._fromTerminal),
                                        _pointsBefore(src._pointsBefore) {}
    };


#endif