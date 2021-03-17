// dice.h
//
// class Dice
//    - Basically an array of size N
//    - contains the digit shown by each die

#ifndef DICE_HPP_INCLUDED
#define DICE_HPP_INCLUDED 1


#include <cassert>
#include <cstdlib> // TODO needed?
#include <array>
#include <stdexcept>

#include "enum.h"


namespace refac
{

using Digit_t = int_least8_t; // Save memory in arrays using smallest type
using Player_t = int_fast8_t;
using Selection_t = std::vector<bool>;
using Points_t = int_fast32_t;

// Class throw represents the rolled digits (of a hypothetical set of
// dice). That means, the dice are represented by the number of
// rolled '1', ..., '6' digits, without representing or ordering
// individual dice.
class Throw
{
  public:
    using Count_t = int_least8_t; // Allow small arrays by using smallest type

  private:
    enum class DigitType // TODO need base type?
    {
      one, two, three, four, five, six, total, // 0..6
      count, begin = 0, end = count, none = -1 // canonical enum members
    };
    // Use vector to allow simple move semantics
    std::vector<Count_t> counts; // len=7. [0-5]=digits, [6]=total.

  public:
    Throw() : counts(DigitType::count, Count_t(0)) {}
    template<typename T,
             std::enable_if_t<std::is_convertible_v<T, decltype(counts)>>
            >
    explicit Throw(T&& t) : counts(std::forward<T>(t))
    {
      assert(counts.size() == DigitType::counts);
    }

    bool any() const; // Any die contained?
    bool operator==(Throw const& other) const = default;
    // TODO comparison faster if comparing total first?
    //      move total to front?

    void fill(Count_t c);
    // TODO needed?
//    void add(DigitType d);
//    void remove(DigitType d);
//    void setDigitType(DigitType d, Count_t newVal);

    Count_t operator[](DigitType d) const;
    Count_t& operator[](DigitType d);
    Count_t total() const;
    Count_t& total();

    // copy + move assign defualt
    // Copy + move ctor default
    // Dtor default
};

// Class Action represents a "raw" action. That means, all the information that
// a RL angent would use to interact with the game.
struct Action
{
    // Represent "none" action by
    //  - t = {0}
    //  - finish = true
    // This would otherwise represent an illegal action.
    Throw throwing;  // Throw subset that is to be rolled again
    bool finish; // Decide whether to re-roll or finish

    // Initialize as "none" action
    Action();
    // Test for virtual "none" action that is used when there are no
    // legal actions to transition to the terminal state with
    // 0 reward.
    bool isNone() const;
    bool operator==(Action const& other) const;
};

class State
{
    // - current points
    // - current dice
    // - unambiguous representation for terminal states

    Throw thrown;
    Points_t points;

  public:

    bool isTerminal() const;

    Points_t takeAction(Action const& action); // Returns current points gained by transition

};

// class Dice represents a set of individual, ordered dice w/o extra
// information.
class Dice
{
    std::vector<Digit_t> dice;

  public:
    static Dice random();

    Digit_t& operator[](int i);
    Digit_t  operator[](int i) const;

    bool operator==(Dice const& rhs) const;

    void roll();
    void roll(Selection_t const& selection);
};

// Class cup represents a set of dice where each die can be active or
// inactive.
class Cup : public Dice
{
  Selection_t active;
};

} // namespace refac

template<unsigned int N>
class Dice
{
    std::array<unsigned int, N> dice = {{0}};

public:
    Dice() = default;
    explicit Dice( std::array<unsigned int, N> const& src) : dice(src) {}

    bool operator==(Dice const& other) { return dice == other.dice; }

    // Roll all or a subset of the dice
    Dice& roll();
    Dice& roll(std::array<bool,N> const& active);

    unsigned int operator[](size_t i) const;
};

#include "dice.inc"

#endif // DICE_HPP_INCLUDED
