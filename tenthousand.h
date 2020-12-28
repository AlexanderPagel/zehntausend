// tenthousand.h
//
// Implementing game mechanics.
// Providing interface requirements of abstract RL classes.
//
// class TenKThrow
//  - Representing thrown dice
//  - For each digit, number of dice showing it
//  - May be empty (representing 0 active/rolled dice)
//
// class TenKState
//  - Represents MDP states
//  - Current throw + current points
//  - Throw empty := terminal state

#ifndef TENTHOUSAND_H_INCLUDED
#define TENTHOUSAND_H_INCLUDED 1

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "dice.h"
#include "rl_action.h"
#include "rl_afterstate.h"
#include "rl_environment.h"
#include "rl_exceptions.h"
#include "rl_state.h"


// FIXME constepxr
#define TURN_MINIMUM 350


namespace from_rl_bases
{

/// @note   No validity or range checks in this class.
class TenKThrow
{
public:
    /// ----------------------------------------------------------------
    ///

    // TODO long term: just use std::vector
    using DiceArray_t = std::array<size_t,7>;
    enum {ONE = 0, TWO, THREE, FOUR, FIVE, SIX, TOTAL};

private:
    /// ----------------------------------------------------------------
    ///
    DiceArray_t* _arr;
    DiceArray_t const& _arrR = *_arr;

protected:
    /// ----------------------------------------------------------------
    ///
    bool isInit() const { return _arr != nullptr; }
    void init() { _arr = new DiceArray_t; }
    void checkInit() const;

public:
    /// ----------------------------------------------------------------
    /// basic object manipulation
    void clear() { if( _arr != nullptr ) delete _arr; }

    /// ----------------------------------------------------------------
    /// logical game functionality
    // Set digits shown by dice
    void fill(size_t v);
    // Does throw contain at least 1 die?
    bool any() const { return (*_arr)[TOTAL] != 0; }
    // Access number of thrown digits (e.g., number of rolled 6s)
    size_t& operator[](size_t fig) { return (*_arr)[fig]; }
    size_t operator[](size_t fig) const { return (*_arr)[fig]; }
    size_t& total() { return (*_arr)[TOTAL]; }
    size_t total() const { return (*_arr)[TOTAL]; }

    // Compare equal if all digits appears equally often
    bool operator==(TenKThrow const& other) const;

    /// ----------------------------------------------------------------
    /// creation and assignments
    TenKThrow& operator=(TenKThrow const& src);
    TenKThrow& operator=(TenKThrow&& src);

    TenKThrow();
    explicit TenKThrow(TenKThrow const& src);
    explicit TenKThrow(TenKThrow&& src);
    explicit TenKThrow(DiceArray_t const& a);

    ~TenKThrow() { this->clear(); }
};

/// ====================================================================
/// Class TenKState = throw + points

class TenKState : protected TenKThrow
{
public:
    using Points_t = unsigned int;
    using Throw_t = from_rl_bases::TenKThrow;

private:
    Points_t _p;    /// \tbc more members slow "inside" moves

public:
    /// ----------------------------------------------------------------
    /// rl:Statae requirements
    auto
    isTerminal() const
        -> bool { return !any(); }

    auto
    clone() const& noexcept(true)
        -> TenKState*
    {
        return new TenKState(*this);
    }

//    auto
//    clone() && noexcept(true)
//    {
//        return new TenKState( std::move(*this) );
//    }

    /// ----------------------------------------------------------------
    /// game-related functionality

    Throw_t&       thrown()        { return *this; }
    Throw_t const& thrown() const  { return *this; }
    Points_t&       points()       { return _p; }
    Points_t const& points() const { return _p; }

    // FIXME remove
    bool terminal() const { return this->isTerminal(); }

    // Initialize to 6 random dice, points = 0
    static TenKState randomStart();

    bool operator==(TenKState const& other) const;

    /// ----------------------------------------------------------------
    /// assignments
    // uses default now
//    TenKState& operator=(TenKState const& src) noexcept;
//    TenKState& operator=(TenKState&& src) noexcept;

    /// ---------------------------------------------------------------
    /// constructors

    TenKState() : Throw_t(), _p(0) {}
    // uses default now
//    TenKState(TenKState const& src);
//    TenKState(TenKState&& src);
    TenKState(Throw_t const& t, Points_t const& p);
};

/// ====================================================================
///
class TenKMove : public rl::Action, protected TenKThrow
{
public:
    using Throw_t = from_rl_bases::TenKThrow;   // derived throw type to be used with this action type

private:
    bool _finishes;

public:
    /// ----------------------------------------------------------------
    /// action requirements

        // empty

    /// ----------------------------------------------------------------
    /// game functionality
    Throw_t& putAside() { return *this; }
    Throw_t const& putAside() const { return *this; }
    bool& finishes() { return _finishes; }
    bool finishes() const { return _finishes; }

    static TenKMove zero() { return TenKMove(); }

    /// ----------------------------------------------------------------
    /// assignments
    // Uses default assignment now!
//    auto
//    operator=(TenKMove const& src)
//        -> TenKMove&
//    {
//        if( this != &src )
//        {
//            Throw_t::operator=(src);
//            _finishes = src._finishes;
//        }
//        return *this;
//    }

//    auto
//    operator=(TenKMove&& src)
//        -> TenKMove&
//    {
//        if( this != &src )
//        {
//            Throw_t::operator=( std::move(src) );
//            _finishes = src._finishes;
//        }
//        return *this;
//    }

    /// ----------------------------------------------------------------
    /// ctors
    TenKMove() : rl::Action(), Throw_t(), _finishes(false) {}
    // Uses default now
//    TenKMove(TenKMove const& src) : rl::Action(), Throw_t(src), _finishes(src._finishes) {}
//    TenKMove(TenKMove&& src) : rl::Action(), Throw_t( std::move(src) ), _finishes(src._finishes) { }

    TenKMove(Throw_t&& t, bool b);
    virtual ~TenKMove() = default;
};

} // namespace from_rl_bases


class illegal_move_error : public runtime_error
{
public:
    illegal_move_error() : runtime_error("") {}
    illegal_move_error(std::string const& s) : runtime_error(s) {}
    // Uses default now
//    ~illegal_move_error() = default;
};

namespace tenthousand_states
{
    class Throw : private std::array<unsigned int, 7>
    {
        typedef typename std::array<unsigned int,7> _base_t;

    public:
        template<typename T>    /// \tbc jesus christ i am so lazy
        void fill( T const& value ) { _base_t::fill(value); }

        unsigned int& operator[](size_t i) { return this->_base_t::operator[](i); }
        unsigned int operator[](size_t i) const {return this->_base_t::operator[](i); }
        bool any() const { return _base_t::operator[](6) != 0; }
        unsigned int&  total() {return _base_t::operator[](6); }
        unsigned int total() const { return _base_t::operator[](6); }

        bool operator==(Throw const& other) const
        {
            return(
               _base_t::operator[](6) == other._base_t::operator[](6) && // in best case, this value is already different
               _base_t::operator[](0) == other._base_t::operator[](0) &&
               _base_t::operator[](1) == other._base_t::operator[](1) &&
               _base_t::operator[](2) == other._base_t::operator[](2) &&
               _base_t::operator[](3) == other._base_t::operator[](3) &&
               _base_t::operator[](4) == other._base_t::operator[](4) &&
               _base_t::operator[](5) == other._base_t::operator[](5)
               );
        }

        Throw& operator=(Throw const&) = default;

        Throw() : _base_t( {0} ) {}
        Throw(Throw const& src) : _base_t( src) {}
        Throw(Throw&& src) : _base_t( src ) {}    // implicit move
        explicit Throw(_base_t&& src) : _base_t(std::move(src)) {}

    };

    /// then, enter terminal with 0 diceLeft is equal to s = terminate state, others
    ///  /e.g., enter terminal wih 4 dice and 200 points is not terminal, but reached after illegal moves
    class State : private std::pair<from_rl_bases::TenKThrow,unsigned int>
    {
        typedef from_rl_bases::TenKThrow Throw_t;
        typedef typename std::pair<Throw_t,unsigned int> _base_t;
    public:
        Throw_t& thrown() { return _base_t::first; }
        Throw_t const& thrown() const {return _base_t::first; }
        unsigned int& points() { return _base_t::second; }
        unsigned int const& points() const { return _base_t::second; }
        bool terminal() const { return !_base_t::first.any(); }

        static State randomStart();

        bool operator==(State const& other) const { return _base_t::second == other.second && _base_t::first == other.first; }

        State& operator=(State const&) = default;

        State() = default;
        State(State const& src) : _base_t( src ) {}
        State(State&& src) : _base_t( std::move(src) ) {}    // implicit move
        explicit State(_base_t&& src) : _base_t( std::move(src) ) {}
    };

    class Move : private std::pair<from_rl_bases::TenKThrow,bool>
    {
        typedef from_rl_bases::TenKThrow Throw_t;
        typedef typename std::pair<Throw_t,bool> _base_t;
    public:
        Throw_t& putAside() { return _base_t::first; }
        Throw_t const& putAside() const { return _base_t::first; }
        bool& finishes() { return _base_t::second; }
        bool const& finishes() const {return _base_t::second; }

        static Move zero() { return Move( Throw_t{{0,0,0,0,0,0,0}},false ); }

        Move& operator=(Move const&) = default;

        Move(Move const& src) : _base_t( src ){}
        Move(Move&& src) : _base_t( src ) {}
        Move(Throw_t&& t, bool b) : _base_t( std::move(t), b) {}

    };

    class Afterstate;

    static Afterstate afterstate(from_rl_bases::TenKState const&, from_rl_bases::TenKMove const& m);

    class Afterstate : private std::pair<unsigned int,unsigned int>
    {
        typedef typename std::pair<unsigned int,unsigned int> _base_t;
        bool _fromTerminal = false;
        unsigned int _pointsBefore = 0;
    public:
        bool& fromTerminal() { return _fromTerminal; }
        inline bool fromTerminal() const {return _fromTerminal; }
        unsigned int& diceLeft() { return this->first; }
        unsigned int diceLeft() const { return this->first; }
        unsigned int& points() { return this->second; }
        unsigned int points() const  { return this->second; }
        unsigned int& pointsBefore() { return this->_pointsBefore; }
        unsigned int pointsBefore() const { return this->_pointsBefore; }

        /// --------------------------------------------------------------------
        /// rl::State requirements
        bool isTerminal() const { return fromTerminal(); }
        Afterstate* clone() const& noexcept { return new Afterstate(*this); }
        Afterstate* clone() && noexcept { return new Afterstate(std::move(*this)); }
        Afterstate& fromSA(rl::State const& s, rl::Action const& a) &
        {
            from_rl_bases::TenKState const& sg = dynamic_cast<from_rl_bases::TenKState const&>(s);
            from_rl_bases::TenKMove const& ag = dynamic_cast<from_rl_bases::TenKMove const&>(a);

            *this = tenthousand_states::afterstate(sg,ag);
            return *this;
        }

        /// --------------------------------------------------------------------
        Afterstate& operator=(Afterstate const& src)
        {
//            rl::Afterstate::operator=(src);
            if( this != &src )
            {
                _fromTerminal = src._fromTerminal;
                _pointsBefore = src._pointsBefore;
                _base_t::first = src._base_t::first;
                _base_t::second = src._base_t::second;
            }
            return *this;
        }

        Afterstate& operator=(Afterstate&& src)
        {
            if( this != &src )
            {
                _fromTerminal = src._fromTerminal;
                _pointsBefore = src._pointsBefore;
                _base_t::first = src._base_t::first;
                _base_t::second = src._base_t::second;
            }
            return *this;
        }

        bool operator==(Afterstate const& other) const
        {
            return
                    (fromTerminal() && other.fromTerminal())    // terminal states are the same
                ||
                    (_base_t::first == other._base_t::first &&
                    _base_t::second == other._base_t::second &&
                    _pointsBefore == other._pointsBefore)
                ;
        }

        Afterstate() = default;
        Afterstate(Afterstate const& src) : _base_t( src ),
                                            _fromTerminal(src._fromTerminal),
                                            _pointsBefore(src._pointsBefore) {}
        Afterstate(Afterstate&& src) :  _base_t( std::move(src) ),
                                        _fromTerminal(src._fromTerminal),
                                        _pointsBefore(src._pointsBefore) {}

    };


}




/// ------------------------------------------------------------------------
/// class Tenthousand ------------------------------------------------------

template<unsigned int P=1>
class Tenthousand
{
private:
    /// \tbc assignment function?
    /// \tbc cannot move std::array on int types?

public:
    typedef unsigned int Player;
    typedef from_rl_bases::TenKThrow Throw_t;
    typedef from_rl_bases::TenKState State_t;
    typedef from_rl_bases::TenKMove Move_t;
    typedef tenthousand_states::Afterstate Afterstate_t;

    typedef typename std::vector<Move_t> MoveVector_t;
    typedef MoveVector_t ActionVector_t;
private:

    Player _player;                                                          // the player who's turn it is
    array<unsigned int, P> _points;                                          // points for each player

    // note: Dice<6> rolls values in [1,6]
    Dice<6> _cup;                                                            // one cup of dice for all players
    unsigned int _current;

    // helping stuff during individual turns
    array<bool,6> _active;                                                   // dice NOT put to the side
    array<unsigned int,8> _aside;                                            // how many of each were put aside from a throw 0-5: dice, 6: TOTAL, 7: ANY
    bool _terminatedLastMove = false;
    static const State_t _terminalState;

    // checks for a triplet of the given figure in the ACTIVE dices position 0 and 3 indicate existence ( == NAN or not)
    std::array<int,6> _triples();  /// \tbc return rref?

    Player _incrementPlayer();
    void _abortTurn(string const& s = "");   // abort with 0 points for the player (throws illegal_move_error)

protected:
    void _refresh(void) &;
    void _setCurrent(unsigned int c) { _current = c; }

public:
    Throw_t thrown() const;   // note: do not put aside befostatere
    State_t state() const;

    /// \return
    ///        0 if move is illegal
    ///        else the current points after the legal move
    static unsigned int legal(State_t const&, Move_t const&);
    static Afterstate_t afterstate(State_t const&, Move_t const& m);
    static auto simulate(State_t const& s, Move_t const& a) -> std::pair<double,State_t>;

    static MoveVector_t legalMoves(State_t const&);
private:
    typedef MoveVector_t (*_legalMovesFunc_t)(State_t const&);
public:
    static constexpr _legalMovesFunc_t legalActions = &legalMoves;
    MoveVector_t legalMoves() const;

    enum Figure {ONE = 0, TWO, THREE, FOUR, FIVE, SIX, TOTAL, ANY, MAX, NAN};

    // API for move input
    void putAside(void);            // put max possible value aside
    void putAside(unsigned int d);  // put one dice aside
    void putAside(Move_t const&) &;   /// \tbc replace arg type Move with type throw. (or overload)
    Dice<6> const& roll(void);      // roll active dice
    unsigned int finishTurn(void);  // stop and save current points
    auto makeMove(Move_t const&) & -> void;

    // API for reading status
    Player const& getPlayer() const { return _player; }
    unsigned int getPoints(Player p) const { return p<=P ? _points[p] : 0; }
    unsigned int getCurrent() const { return _current; }
    unsigned int activeDice() const { return 6 - _aside[TOTAL]; }
    bool getPutAny() const { return (bool)_aside[ANY]; }

    /// ------------------------------------------------------------------------
    /// requirements from rl::Environment
    auto
    init()
        -> Tenthousand<P>&;

    auto
    takeAction(const rl::Action&)
        -> double;

    auto
    cloneState() const
        -> shared_ptr<rl::State>;

    /// ------------------------------------------------------------------------
    /// ctor, dtor
    Tenthousand();  /// \tbc don't start game in constructor, wait for a function call like start()
    ~Tenthousand() = default;

    void print() const;
};

/// ------------------------------------------------------------------------
/// implementation ---------------------------------------------------------

template<unsigned int P>
const typename Tenthousand<P>::State_t Tenthousand<P>::_terminalState = State_t();

template<unsigned int P>
std::array<int,6>
Tenthousand<P>::_triples()
{
    // compute the figures which ARE shown three times
    int tr[6] = {0};
    unsigned int tripleFigures[2] = {NAN, NAN}; int second = 0;
    for(int pos = 0; pos < 6; ++pos)
    {
        if( _active[pos] )
        {
            unsigned int f = _cup[pos];
            if( (++tr[f]) % 3  == 0)
                tripleFigures[second++] = f;
        }
    }

    // - tripleFigures contains the figures of both potential triplets, or -1 otherwise
    std::array<int,6> triples{ -1, -1, -1, -1, -1, -1 };

    // find the exact position of the corresponding dice
    if( tripleFigures[0] != NAN )
    {
        int i = 0;
        for(; _cup[i] != tripleFigures[0] || _active[i] == false; ++i )
            ;
        triples[0] = i;

        for( ++i; _cup[i] != tripleFigures[0] || _active[i] == false; ++i )
            ;
        triples[1] = i;

        for( ++i; _cup[i] != tripleFigures[0] || _active[i] == false; ++i )
            ;
        triples[2] = i;

        if( tripleFigures[1] != NAN )
        {
            // - pos == 4 at this point
            // search backwards to avoid collision in case of identical figures (sextuple)
            i = 5;
            for(; _cup[i] != tripleFigures[1] || _active[i] == false; --i )
                ;
            triples[3] = i;
            for( --i; _cup[i] != tripleFigures[1] || _active[i] == false; --i )
                ;
            triples[4] = i;
            for( --i; _cup[i] != tripleFigures[1] || _active[i] == false; --i )
                ;
            triples[5] = i;
        }
    }

    return triples;
}

template<unsigned int P>
typename Tenthousand<P>::Player
Tenthousand<P>::_incrementPlayer()
{
    return _player = (_player +1 ) % P;
}

template<unsigned int P>
void
Tenthousand<P>::_abortTurn(std::string const& s)
{
    _refresh();
    throw illegal_move_error{s};   // notify caller
    return;
}

template<unsigned int P>
auto
Tenthousand<P>::thrown() const -> Throw_t
{
    Throw_t t;
    t.fill(0);

    for( unsigned int i = 0; i < 6; ++i )
    {
        if( _active[i] )
        {
            ++t[ _cup[i] ];
            ++t[ TOTAL ];
        }
    }

    return t;
}

template<unsigned int P>
auto
Tenthousand<P>::state() const -> State_t
{
    // : special case for generic interfaces
    if( this->_terminatedLastMove )
        return _terminalState;


    State_t test( thrown(), _current );
    return test;
}

template<unsigned int P>
unsigned int
Tenthousand<P>::legal(State_t const& s, Move_t const& m)
{
    if( s.terminal() )
    {
        return 0;
    }

    // : test any
    if( m.putAside()[TOTAL] == 0 )
        return 0;

    // : test not too much
    for( unsigned int i = 0; i < 6; ++i )
    {
        if( s.thrown()[i] < m.putAside()[i] )
            return 0;
    }

    // : test in triples
    for( unsigned int ordinaryFigure : std::array<unsigned int, 4>({2,3,4,6}) )
        if( (m.putAside()[ordinaryFigure - 1] % 3) != 0 )   // zero based index
            return 0;

    // : calculate gain
    unsigned int gain = 0;
    for(int i = 1; i <= 6; ++i)
    {

        unsigned int n               = m.putAside()[i-1],
                     triple_count    = n / 3,
                     triple_leftover = n - 3 * triple_count;

        // : increase points for each triple
        gain += i * 100 * triple_count;
        // : for one, increase triple value and count solo dice
        if( i == 1 )
        {
         gain += 900 * triple_count;
         gain += 100 * triple_leftover;
        }
        // : for five, count solo figures
        else if( i == 5 )
        {
            gain += 50 * triple_leftover;
        }
    }

    // : test score after the move
    unsigned int tempScore = s.points() + gain;
    if ( m.finishes() && tempScore < TURN_MINIMUM )
        return 0;   // finishes, but not enough points

    return tempScore;    // rolls again or has enough points
}

template<unsigned int P>
auto
Tenthousand<P>::afterstate(State_t const& s, Move_t const& m) -> Afterstate_t
{
    Afterstate_t as;

    // : special case for afterstates arising from terminal states s
    if( s.terminal() )
    {
        as.diceLeft() = as.points() = as.pointsBefore() = 0;    // \tbc should be std. initialized
        as.fromTerminal() = true;
        return as;
    }

    // - s is a regular, non-terminal state
    as.pointsBefore() = s.points();

    as.fromTerminal() = false;

    // : special case for known-ending afterstates
    unsigned int c = legal(s, m);   // return current points after move if legal (keeping points when finishing)
    as.points() = c;    // points "afterwards": 0 if illegal, the points else

    // : dice left
    if( c == 0 || m.finishes() )
    {
        // - either m is aborting or m finishes
        as.diceLeft() = 0;
        return as;
    }
        // - m is regular move rolling
    as.diceLeft() = s.thrown().total() - m.putAside().total();
    if( as.diceLeft() == 0 )
        as.diceLeft() = 6;  // value indicates ending afterstates, so should not be reset to 6 for finising moves

    return as;
}

template<unsigned int P>
auto
Tenthousand<P>::simulate(State_t const& s, Move_t const& a) -> std::pair<double,State_t>
{
    unsigned int pointsAfter = legal(s, a);

    if( pointsAfter == 0 )  // a is illegal or s terminal
    {
        double r;
        // : a illegal
        if( s.terminal() )
            r = 0.0;
        else
            r = - (double)s.points();   // points is unsigned
        // : return terminal (unlocked) state
        return std::pair<double,State_t>(r, State_t()); /// \tbc zero function
    }

    // : compute next state from afterstate!
    Afterstate_t as( afterstate(s,a) );


    // : set points
    State_t s_;
    s_.points() = pointsAfter;

    // : set thrown dice (randomly)
    s_.thrown().fill(0);
    s_.thrown()[TOTAL] = as.diceLeft(); // 0 to 6 depending on s, a
    for( unsigned int i = 0; i < s_.thrown()[TOTAL]; ++i)
    {
        ++s_.thrown()[ std::rand() % 6];    // roll one random number
    }

    // note: points() are unsigned, so cast is necessary for negative returns
    return std::pair<double,State_t>((double)s_.points() - s.points(), s_);
}

template<unsigned int P>
auto
Tenthousand<P>::legalMoves(State_t const& s) -> MoveVector_t
{
    MoveVector_t movVec;
    if( s.terminal() )
    {
        return movVec;
    }

    unsigned int f[6];

    for( f[SIX]   = 0; f[SIX]   <= s.thrown()[SIX];   f[SIX]   += 3 )
    for( f[FIVE]  = 0; f[FIVE]  <= s.thrown()[FIVE];  f[FIVE]  += 1 )
    for( f[FOUR]  = 0; f[FOUR]  <= s.thrown()[FOUR];  f[FOUR]  += 3 )
    for( f[THREE] = 0; f[THREE] <= s.thrown()[THREE]; f[THREE] += 3 )
    for( f[TWO]   = 0; f[TWO]   <= s.thrown()[TWO];   f[TWO]   += 3 )
    for( f[ONE]   = 0; f[ONE]   <= s.thrown()[ONE];   f[ONE]   += 1 )
    {
        unsigned int sum = f[ONE]+f[TWO]+f[THREE]+f[FOUR]+f[FIVE]+f[SIX];
        Move_t m( Throw_t({f[ONE],f[TWO],f[THREE],f[FOUR],f[FIVE],f[SIX],sum}), true );
        if( legal(s, m))    /// \tbc SOME double checks still here
            movVec.push_back( m );  // do NOT move        m.finishes() = false;
        if( legal(s, m))
            movVec.push_back( std::move(m) );
    }
    return movVec;
}

template<unsigned int P>
auto
Tenthousand<P>::legalMoves() const -> MoveVector_t
{
    return MoveVector_t(
                        legalMoves(this->state())   // is an rvalue, so move constructor is used if possible
                        );
}


template<unsigned int P>
void
Tenthousand<P>::_refresh(void) &
{
    _current = 0;
    _active.fill(true);
    _aside.fill(0);
    _incrementPlayer();
    _cup.roll(); // roll all dice
}

template<unsigned int P>
void
Tenthousand<P>::putAside()
{
    // put triplets aside
    auto trp( _triples() );      // move

    if( trp[0] != -1 )
    {
        putAside(trp[0]);
        putAside(trp[1]);
        putAside(trp[2]);
        if( trp[3] != -1 )
        {
            putAside(trp[3]);
            putAside(trp[4]);
            putAside(trp[5]);
        }
    }

    // put all 1s and 5s aside
    for(unsigned int d = 0; d < 6; ++d)
    {
        if( _active[d] && (_cup[d] == ONE || _cup[d] == FIVE ) )
            putAside(d);
    }
}

template<unsigned int P>
void
Tenthousand<P>::putAside(unsigned int d)
{
    // validate input to be legit
    if( d > Figure::SIX ) _abortTurn("You can put dice 0-5 aside only.");
    if( !_active[d] )     _abortTurn("Die already aside.");    /// \tbc throw other exception that allows to re-consider the move instead of aborting

    // put dice aside
    _active[d] = false;
    unsigned int fig = _cup[d];
    ++_aside[fig];
    ++_aside[TOTAL];
    _aside[ANY] = 1; // 1 == true

    // increase current when 1 or 5
    if     ( fig == ONE )  _current += 100;
    else if( fig == FIVE ) _current +=  50;

    // check for triplets
    if( (_aside[fig]%3) == 0 )
    {
        if( fig == ONE )
            _current += 700;
        else if( fig == FIVE )
            _current += 350;
        else
            _current += (fig+1) * 100;   // e.g. fig == THREE has integer value 2
    }
}

template<unsigned int P>
void
Tenthousand<P>::putAside(Move_t const& m) &
{
    Move_t temp( m );  // copy construct
    for( int pos = 0; pos < 6; ++pos )
    {
        unsigned int figure = _cup[pos];
        bool canPut  = _active[pos],
             wantPut = temp.putAside()[ figure ];
        // ... if figure at position pos is active and that figure is in the move ...
        if( canPut && wantPut )
        {
            putAside(pos);      /// \tbc ineffective: double checking
            --temp.putAside()[ figure ];
        }
    }
}

template<unsigned int P>
Dice<6> const&
Tenthousand<P>::roll()
{
    if( _aside[TWO]   % 3 ||
        _aside[THREE] % 3 ||
        _aside[FOUR]  % 3 ||
        _aside[SIX]   % 3)
    {
        _abortTurn("Can only put these aside in triplets.");    // throws warning. catch with decision logic
    }

    if( _aside[TOTAL] == 6 )
    {
        _aside.fill(0);
        _active.fill(true);
    }
    else if( _aside[ANY] == 0)
        _abortTurn("Gotta put something aside.");    // alternatively finish turn
    else
    {
        auto temp = _aside[TOTAL];
        _aside.fill(0);
        _aside[TOTAL] = std::move(temp);
    }

    return _cup.roll(_active);
}

template<unsigned int P>
typename Tenthousand<P>::Player
Tenthousand<P>::finishTurn()
{
    if( _aside[TWO]   % 3 ||
        _aside[THREE] % 3 ||
        _aside[FOUR]  % 3 ||
        _aside[SIX]   % 3)
    {
        _abortTurn("You can only put these aside in groups of 3."); // throws to inform caller --> not catching informs player logic
    }
    // Any points for you?
    if( _aside[ANY] == 1 )   // 0 == false
    {
        // 350 rule - or - REALLY any points for you?
        /// \tbc 350 rule can be before aside[ANY] test as well, but then error messages would change slightly

        if( _current < TURN_MINIMUM)
            _abortTurn("Gotta try harder to reach the turn limit.");

        _points[_player] += _current;
    }
    else
        /// \tbc later: introduce a new exception to indicate terminating states.
        ///       Then derive illegal_move from that error. That way the caller
        ///       can distinguish between regular termination (generating reward from finishing), and
        ///       violent finishing (losing all points)
        ///
        /// for now, use abortTurn to throw illegal_move_error whenever all points of the turn are lost
        /// ...
        /// or sth like that, maybe, or not?
        _abortTurn("Nothing put aside.");


    _current = 0;
    _active.fill(true);
    _aside.fill(0);

    // rtd
    _cup.roll();



    return _incrementPlayer();
}

template<unsigned int P> auto
Tenthousand<P>::makeMove(Move_t const& m) & -> void
{
//    if( legal(this->state(), m ) == 0 )
//        throw illegal_move_error("can not make this move");

    putAside(m);
    if( !m.finishes() )
    {
        roll();
        return;
    }
    else
        finishTurn();

}

template<unsigned int P>
auto
Tenthousand<P>::init() -> Tenthousand<P>&
{
    _player = 0;
    _points.fill(0);
    _cup.roll(); // roll all dice
    _current = 0;
    _active.fill(true);
    _aside.fill(0);
    _terminatedLastMove = false;
    return *this;
}

template<unsigned int P>
auto
Tenthousand<P>::takeAction(rl::Action const& a) -> double
{
    Move_t const& m = dynamic_cast<Move_t const&>(a);

    State_t s( this->state() );     // copy-construct state from the game
    try
    {
        makeMove(m);                    // take action a in current game
    }
    catch( illegal_move_error const& err )
    {
        _terminatedLastMove = true;
        return -(double)s.points();
    }
    catch( ... )
    {
        cout << "no fucking way man." << endl;
        exit(5431415);
    }

    // : go on computing the reward as simulation of the same move (foir now) \tbc

    unsigned int pointsAfter = legal(s, m);

    if( pointsAfter == 0 )  // a is illegal or s terminal
    {
        double r;
        // : a illegal
        if( s.terminal() )
            r = 0.0;
        else
            r = - (double)s.points();   // points is unsigned
        // : return terminal (unlocked) state
        _terminatedLastMove = true;
        return r;
    }

    // : compute next state from afterstate!
    Afterstate_t as( afterstate(s,m) );

    // : set points
    State_t s_;
    s_.points() = pointsAfter;

    // : set thrown dice (randomly)
    s_.thrown().fill(0);
    s_.thrown()[TOTAL] = as.diceLeft(); // 0 to 6 depending on s, a
    for( unsigned int i = 0; i < s_.thrown()[TOTAL]; ++i)
    {
        ++s_.thrown()[ std::rand() % 6];    // roll one random number
    }

     _terminatedLastMove = m.finishes();

    // note: points() are unsigned, so cast is necessary for negative returns
    return (double)s_.points() - s.points();
}

template<unsigned int P>
auto
Tenthousand<P>::cloneState() const -> shared_ptr<rl::State>
{
    State_t* temp;
    if( _terminatedLastMove )
        temp = new State_t(_terminalState);
    else
        temp = new State_t(this->state());     // move construct/RVO

    shared_ptr<rl::State> sPtr{ dynamic_cast<rl::State*>(temp) };
    return sPtr;
}

template<unsigned int P>
Tenthousand<P>::Tenthousand()
    : _player(0),  _points(), _cup(), _current(0), _active(), _aside()
{
    _points.fill(0); // reset points
    _active.fill(true);
    _aside.fill(0);
    _cup.roll(); // randomize dice
}

template<unsigned int P>
void Tenthousand<P>::print() const
{
//    std::cout << "Tenthousand (" << P << " players):" << endl;
    for(unsigned int i = 0; i < P; ++i)
        std::cout << "Player [" << setw(P>9?2:1) << i << "]:" << setw(6) << _points[i] << endl;
    cout << endl;

    if( !getPutAny() )
    {
        cout << "      \n"
                "     |\t    Dice:    1   2   3   4   5   6\n"
                "  " << setw(2) << _player << " |"
                      "\t   +-----+\n"
                "     |\t   |" << setw(5) << _current << "|  ";
        if( _active[0] ) cout << "[" << _cup[0]+1 << "] "; else cout << " " << _cup[0]+1 << "  ";
        if( _active[1] ) cout << "[" << _cup[1]+1 << "] "; else cout << " " << _cup[1]+1 << "  ";
        if( _active[2] ) cout << "[" << _cup[2]+1 << "] "; else cout << " " << _cup[2]+1 << "  ";
        if( _active[3] ) cout << "[" << _cup[3]+1 << "] "; else cout << " " << _cup[3]+1 << "  ";
        if( _active[4] ) cout << "[" << _cup[4]+1 << "] "; else cout << " " << _cup[4]+1 << "  ";
        if( _active[5] ) cout << "[" << _cup[5]+1 << "]" << endl; else cout << " " << _cup[5]+1 << "  " << endl;
        cout << "      \t   +-----+\n" << endl;
    }
    else
    {
        cout << "      \n"
                "     |\t    Dice:    1   2   3   4   5   6\n"
                "  " << setw(2) << _player << " |\n"
                "     |\t    " << setw(5) << _current << "   ";
        if( _active[0] ) cout << "[" << _cup[0]+1 << "] "; else cout << " " << _cup[0]+1 << "  ";
        if( _active[1] ) cout << "[" << _cup[1]+1 << "] "; else cout << " " << _cup[1]+1 << "  ";
        if( _active[2] ) cout << "[" << _cup[2]+1 << "] "; else cout << " " << _cup[2]+1 << "  ";
        if( _active[3] ) cout << "[" << _cup[3]+1 << "] "; else cout << " " << _cup[3]+1 << "  ";
        if( _active[4] ) cout << "[" << _cup[4]+1 << "] "; else cout << " " << _cup[4]+1 << "  ";
        if( _active[5] ) cout << "[" << _cup[5]+1 << "]" << endl; else cout << " " << _cup[5]+1 << "  " << endl;
        cout << "      \n" << endl;
    }
}

namespace std
{

    template<> struct hash<from_rl_bases::TenKState>
    {
        size_t operator()(from_rl_bases::TenKState const& x) const
        {
            return std::hash<unsigned int>()
            (
                x.terminal()
                    ?
                        (
                            1 << (sizeof(unsigned int)*8 -1)
                        )
                    :
                        (
                            x.thrown()[0] << 0  |
                            x.thrown()[1] << 3  | // max size is 8 ^= 3 bit
                            x.thrown()[2] << 6  |
                            x.thrown()[3] << 9  |
                            x.thrown()[5] << 12 |
                            x.thrown()[4] << 15 |
                            x.thrown()[6] << 18 |
                            x.  points()    << 21
                         )
             ); // return
        }   // operator()
    };   // struct hash<>

    template <> struct hash<tenthousand_states::State>
    {
        size_t operator()(tenthousand_states::State const& x) const
        {
            return std::hash<unsigned int>()
            (
                x.terminal()
                    ?
                        (
                            1 << (sizeof(unsigned int)*8 -1)
                        )
                    :
                        (
                            x.thrown()[0] << 0  |
                            x.thrown()[1] << 3  | // max size is 8 ^= 3 bit
                            x.thrown()[2] << 6  |
                            x.thrown()[3] << 9  |
                            x.thrown()[5] << 12 |
                            x.thrown()[4] << 15 |
                            x.thrown()[6] << 18 |
                            x.  points()    << 21
                         )
             );
        }
    };

    template <> struct hash<tenthousand_states::Afterstate>
    {
        size_t operator()(tenthousand_states::Afterstate const& x) const
        {
            return std::hash<unsigned int>()
            (
                x.fromTerminal()  ? (   // 0/0 states
                                        1 << (sizeof(unsigned int)*8 -1)
                                    )
                                  :
                                    (
                                        x.diceLeft()    <<   0 |
                                        x.points()      <<   3 |
                                        x.pointsBefore() << 15  /// reserving 12 bit for 4095 points /// \tbc there is a max value here after which it can get ineffective
                                    )
             );
        }
    };

}

namespace tenthousand_states
{

        static Afterstate afterstate(from_rl_bases::TenKState const& s, from_rl_bases::TenKMove const& m)
    {
        Afterstate as;

        // : special case for afterstates arising from terminal states s
        if( s.terminal() )
        {
            as.diceLeft() = as.points() = as.pointsBefore() = 0;    // \tbc should be std. initialized
            as.fromTerminal() = true;
            return as;
        }

        // - s is a regular, non-terminal state
        as.pointsBefore() = s.points();

        as.fromTerminal() = false;

        // : special case for known-ending afterstates
        unsigned int c = Tenthousand<1>::legal(s, m);   // return current points after move if legal (keeping points when finishing)
        as.points() = c;    // points "afterwards": 0 if illegal, the points else

        // : dice left
        if( c == 0 || m.finishes() )
        {
            // - either m is aborting or m finishes
            as.diceLeft() = 0;
            return as;
        }
            // - m is regular move rolling
        as.diceLeft() = s.thrown().total() - m.putAside().total();
        if( as.diceLeft() == 0 )
            as.diceLeft() = 6;  // value indicates ending afterstates, so should not be reset to 6 for finising moves

        return as;
    }

}

#endif // TENTHOUSAND_H_INCLUDED
