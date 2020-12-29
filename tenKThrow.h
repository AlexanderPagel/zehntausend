#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>


#ifndef TENKTHROW_H_INCLUDED
#define TENKTHROW_H_INCLUDED 1

/// @note   No validity or range checks in this class.
class TenKThrow
{
public:
    using DiceArray_t = std::array<size_t,7>;
    enum {ONE = 0, TWO, THREE, FOUR, FIVE, SIX, TOTAL};

private:
    DiceArray_t* _arr;

protected:
    // TODO Use these
    bool isInit() const { return _arr != nullptr; }
    void init() { _arr = new DiceArray_t; }
    void checkInit() const;

public:
    // Access
    void clear() { if( _arr != nullptr ) delete _arr; }
    void fill(size_t v);
    size_t& operator[](size_t fig)       { return (*_arr)[fig]; }
    size_t  operator[](size_t fig) const { return (*_arr)[fig]; }
    size_t& total()       { return (*_arr)[TOTAL]; }
    size_t  total() const { return (*_arr)[TOTAL]; }

    // Content checks
    bool any() const { return (*_arr)[TOTAL] != 0; }
    bool operator==(TenKThrow const& other) const;

    // Copy & assign
    TenKThrow& operator=(TenKThrow const& src);
    TenKThrow& operator=(TenKThrow&& src);
    // TODO need default?  TenKThrow() : _arr( nullptr ){}
    TenKThrow(TenKThrow const& src) : _arr( new DiceArray_t(*src._arr) ) {}
    TenKThrow(TenKThrow&& src) : _arr(src._arr) { src._arr = nullptr; }

    // Construct
    explicit TenKThrow(DiceArray_t const& a) : _arr{ new DiceArray_t(a) } {}
    TenKThrow() : _arr( new DiceArray_t ){ _arr->fill(0); }
    ~TenKThrow() { this->clear(); }
};

#endif //TENKTHROW_H_INCLUDED 