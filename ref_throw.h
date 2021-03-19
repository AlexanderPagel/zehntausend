// ref_throw.h
//
// Implement class "Throw" to represent (unordered) sets of thrown dice. This
// is used in the RL-style implementation of the environment.

#ifndef REF_THROW_H_INCLUDED
#define REF_THROW_H_INCLUDED 1


#include <cassert>

#include "ref_types.h"


namespace refac
{

// Class throw represents the rolled digits (of a hypothetical set of
// dice). That means, the dice are represented by the number of
// rolled '1', ..., '6' digits, without representing or ordering
// individual dice.
class Throw
{
  public:
    using Count_t = int_least8_t; // Allow small arrays by using smallest type

  private:
    // Use vector to allow simple move semantics
    std::vector<Count_t> counts; // len=7. [0-5]=digits, [6]=total.

    template<typename T,
             std::enable_if_t<std::is_convertible_v<T, decltype(counts)>>
            >
    explicit Throw(T&& t) : counts(std::forward<T>(t))
    {
      assert(counts.size() == DigitType::counts);
      assert(consistent());
    }

    Count_t& operator[](DigitType d);
    Count_t& at(DigitType d); // No bounds checking
    Count_t& total(); // TODO needed?

  public:
    Throw(); // Initialize as empty
    explicit Throw(Count_t dieCount, DigitType initialDigit = DigitType::one);

    bool any() const; // Any die contained?
    bool empty() const;
    bool operator==(Throw const& other) const = default;
    Count_t getDigitCount(DigitType d) const;
    bool consistent() const; // Entries non-negative, total is sum of parts
    // TODO comparison faster if comparing total first?
    //      move total to front?

    void fill(Count_t c);
    void setDigitCount(DigitType d, Count_t newValue);
    void add(DigitType d, Count_t c = 1);
    void remove(DigitType d, Count_t c = 1);
    void increment(DigitType d);
    void decrement(DigitType d);

    void roll(); // Re-radomize current number of dice
    void roll(Count_t c); // Randomize c dice

    Throw& operator-=(Throw const&);

    Count_t operator[](DigitType d) const;
    Count_t total() const;

//    static auto digitTypeToDigit(DigitType d) -> decltype(raw(d))

    // copy + move assign defualt
    // Copy + move ctor default
    // Dtor default
};

} // namespace refac


#endif // REF_THROW_H_INCLUDED
