#ifndef TENKERR_H_INCLUDED
#define TENKERR_H_INCLUDED 1

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>


class illegal_move_error : public std::runtime_error
{
public:
    illegal_move_error() : runtime_error("") {}
    illegal_move_error(std::string const& s) : runtime_error(s) {}
    ~illegal_move_error() = default;
};


#endif
