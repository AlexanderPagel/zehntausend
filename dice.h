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

using Digit_t = int_least8_t; // In range [1, 6].
using Player_t = int_fast8_t;
using Selection_t = std::vector<bool>;
using Points_t = int_fast32_t;
// TODO typedef Active_t = bool?

// TODO do we want to move this somewhere else? into a class?
int constexpr totalGameDieCount{6};

// While Digit_t is 1-based and the value represents a digit, DigitType is
// 0-based (to index arrays easily). Use typed enum to avoid mixing the two
// accidentally.
enum class DigitType : Digit_t
{
  one, two, three, four, five, six, total, // 0..6
  count, begin = 0, end = count, none = -1 // canonical enum members
};

// Convert 1-based, valid digit to corresponding digit type
DigitType digitToDigitType(Digit_t   digit);
Digit_t   digitTypeToDigit(DigitType digit);

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

// Class Action represents a "raw" action. That means, all the information that
// a RL angent would use to interact with the game.
// TODO it might be faster to encode the resulting points in the
//      Action object rather than computing them separately. However,
//      that would somewhat break our nice RL setup.
class Action
{
    // TODO need to encapsulate like the "Throw" class?
  public:
    // TODO need "resign" action when an episode is a forced loss?

    static Action makeNone();
    static Action makeWelp();

    // Represent "none" action by the default values:
    //  - t = {0}
    //  - finish = false
    // This pattern can never represent any (other) legal action.
    Throw taking{};  // Throw subset that is to be scored
    bool finish{false}; // Decide whether to re-roll or finish

    // Test for virtual "none" action that is used when there are no
    // legal actions to transition to the terminal state with
    // 0 reward.
    bool isNone() const;
    bool isWelp() const;
    bool operator==(Action const& other) const;

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

class State
{
    // - current points
    // - current dice
    // - unambiguous representation for terminal states
    // Represent terminal states as
    //  - thrown = {0}
    //  - points = 0

    Throw thrown{}; // TODO derive for operator-= etc?
                    //      Also for empty()
    Points_t points{0};

    State(Throw const&, Points_t const&);

  public:
    static State startState(); // Randomize 6 dice, points 0
    static State makeTerminalState();

    bool isTerminal() const;
    bool operator==(State const& other) const;
    Points_t getPoints() const;
    Throw const& getThrown() const;
    void addPoints(Points_t);
    // NOTE Behaves different to Throw::roll()
    void roll(Count_t n); // Randomize remaining dice ('n' new if 0 remaining)

    // Remove some dice without altering points
    void operator-=(Throw const&);

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

// Implements the game rules and state transitions.
// For legal inputs only!
class Environment
{
    State state;

    // Provide a list of legal actions at every time step.
    // TODO We might want to change this later, if appropriate. The only
    //      optimization case should be self-play training.
    std::vector<Action> legalActions;

    void clearActions(); // TODO probably we want to remove w/o resizing vector (?)
    void fillActions(); // Refills the actions vector (no extra clear needed)

  public:
    Environment(); // Initialize in a normal start state

    // Take known-legal action, return points gained
    Points_t takeAction(Action const& action); // Return immerdiate reward
    bool episodeFinished() const;
    State const& getState() const;
    std::vector<Action> const& getLegalActions() const;
    void restart();

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

// Class Dice represents a set of individual, ordered dice w/o extra
// information.
// No optimization needed for this class.
class Dice
{
    // TODO Derive instead? size, empty, ...
    std::vector<Digit_t> dice; // digits in range [1, 6]

  public:
    explicit Dice(Count_t dieCount, Digit_t initialDigit = 1); // Instantiate c dice

    Digit_t  operator[](int pos) const;
    Digit_t& operator[](int pos);

    bool operator==(Dice const&) const = default;

    void roll();
    void roll(Selection_t const& selection);

    void addDie(Digit_t);
};

// Maintain a consistent triple of "Throw", "Dice" and "Selection_t" objects.
// Throw:
//  - getters
// Dice:
//  - getters
//  - setters
// Throw setters would be ambiguous.
//
// The "Throw" part only represents the subset of *active* dice.
//
// This class is for game-style user interaction and does not require
// optimization.
class Cup
{
    Dice dice;          // All dice regardless of activeness
    Throw thrown;       // Only active dice
    Selection_t active; // Activeness mask for member 'dice'

  public:
    explicit Cup(Count_t dieCount, Digit_t initialDigit = 1);

    bool operator==(Cup const&) const; // Exact match of digits, activeness, and positions
    bool operator==(Throw const&) const; // Only active dice, disregarding permutations

    Count_t anyCount() const;
    Count_t activeCount(DigitType d) const;
    Count_t inactiveCount() const;

    std::pair<Digit_t, bool> getDie(int pos) const;
    void setDie(int pos, Digit_t);              // Re-assign existing die digit
    void setDie(int pos, Digit_t, bool active); // Re-assign existing die digit + activeness
    void addDie(Digit_t, bool newActive = true);   // Add new die to the end
    void addDie(DigitType, bool newActive = true);
    void setActive(int pos, bool newActive = true);
    void toggleActive(int pos);
    Selection_t const& getActive() const;

    void roll(); // Randomize all active

    // copy + move ctor default
    // copy + move assign default
    // dtor default
};

// This class is the internal representation of a game-like state. Inputs MUST
// be suitable to the situation, i.e., represent legal actions for the
// represented game.
class GameState
{
    // Internally run a normal Simulation in the 'environment' object. After
    // each change, adjust the cup by-hand to stay consistent with the new
    // state.
    Environment environment;
    Cup cup;
    Action action{Action::makeWelp()};  // Action in construction

    // Replace active dice by a random permutation of the argument. Makes
    // member 'cup' consistent to the member 'state' as long as the number of
    // active dice in cup matches the number of dice in the state.
    void makeDigitsConsistent();

  public:
    GameState();

    // Internal consistency check
    bool consistent() const; // TODO needed for debugging?

    // Get game information
    Environment const& getEnvironment() const;
    Cup    const& getCup   () const;
    State  const& getState () const; // Ignoring dice put aside in current
    Action const& getAction() const;
    bool isTerminal() const;

    // Interact with game
    void toggleAside(pos);
    void selectActionFinish(bool finish);
    void finishTurn(); // Call selectActionFinish beforehand
    void finishTurn(bool finish);
    void restart(); // Initialize new game

    // Copy + move ctor default
    // Copy + move assign default
    // Dtor default
};

// Game-like (incremental) and sanity-checked implementation of the (episodic)
// one-player game. This class will reject actions in a terminal state as
// illegal.
// Only the game-related members (toggle, roll, finish) are sanity checked.
class Game
{
    GameState gameState;
    Selection_t usable;  // Not yet scored dice

    bool scanLegalActions(Action const&) const; // Return true if action is legal
    void adjustUsable(); // Set usable <- current active dice
    void takeConstructedAction(); // Take action and adjust the usable vector to new state

    // Helper because roll() and finish() are almost identical otherwise.
    // Returns false if intended action is illegal (can be passed as return
    // from roll() and finish()).
    bool endSubturnWith(bool finish);

  public:
    Game();

    bool isTerminal() const; // Terminal = no further interactions allowed
    Points_t getReturn() const; // Get episode return in terminal states

    // Interactions return "true" if the interaction is successfull, otherwise,
    // they return "false" and might leave the object in a corrupted state. In
    // this case, restart() must be called before using the object otherwise.
    bool toggleAside(int pos);
    bool roll();    // Roll remaining dice
    bool finish();  // Finish with current selection

    // Initialize new game. Recovers from corrupted states.
    void restart();
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
