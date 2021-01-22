#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <tuple>
#include <vector>

/// ------------------------------------------------------------------------
/// implementation ---------------------------------------------------------

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
    State_t test( thrown(), _current );
    return test;
}

template<unsigned int P>
unsigned int
Tenthousand<P>::legal(State_t const& s, Move_t const& m)
{
    if( s.isTerminal() )
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
    if( s.isTerminal() )
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
        if( s.isTerminal() )
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
    if( s.isTerminal() )
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
            movVec.push_back( std::move(m) );
        m.finishes() = false;
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
        if (_points[_player] >= 10000)
        {
            std::cout <<"You win!" << std::endl;
            //ToDo: Add some functionality to finish game/start a new game/continue playing.
        }
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
Tenthousand<P>::Tenthousand()
    : _player(0),  _points(), _cup(), _current(0), _active(), _aside()
{
    // TODO This shoudld probably be a "reset" or "init" member
    _points.fill(0); // reset points
    _active.fill(true);
    _aside.fill(0);
    _cup.roll(); // randomize dice
}

template<unsigned int P>
void Tenthousand<P>::print() const
{
//    std::cout << "Tenthousand (" << P << " players):" << std::endl;
    for(unsigned int i = 0; i < P; ++i)
        std::cout << "Player [" << std::setw(P>9?2:1) << i << "]:" << std::setw(6) << _points[i] << std::endl;
    std::cout << std::endl;

    if( !getPutAny() )
    {
        std::cout << "      \n"
                "     |\t    Dice:    1   2   3   4   5   6\n"
                "  " << std::setw(2) << _player << " |"
                      "\t   +-----+\n"
                "     |\t   |" << std::setw(5) << _current << "|  ";
        if( _active[0] ) std::cout << "[" << _cup[0]+1 << "] "; else std::cout << " " << _cup[0]+1 << "  ";
        if( _active[1] ) std::cout << "[" << _cup[1]+1 << "] "; else std::cout << " " << _cup[1]+1 << "  ";
        if( _active[2] ) std::cout << "[" << _cup[2]+1 << "] "; else std::cout << " " << _cup[2]+1 << "  ";
        if( _active[3] ) std::cout << "[" << _cup[3]+1 << "] "; else std::cout << " " << _cup[3]+1 << "  ";
        if( _active[4] ) std::cout << "[" << _cup[4]+1 << "] "; else std::cout << " " << _cup[4]+1 << "  ";
        if( _active[5] ) std::cout << "[" << _cup[5]+1 << "]" << std::endl; else std::cout << " " << _cup[5]+1 << "  " << std::endl;
        std::cout << "      \t   +-----+\n" << std::endl;
    }
    else
    {
        std::cout << "      \n"
                "     |\t    Dice:    1   2   3   4   5   6\n"
                "  " << std::setw(2) << _player << " |\n"
                "     |\t    " << std::setw(5) << _current << "   ";
        if( _active[0] ) std::cout << "[" << _cup[0]+1 << "] "; else std::cout << " " << _cup[0]+1 << "  ";
        if( _active[1] ) std::cout << "[" << _cup[1]+1 << "] "; else std::cout << " " << _cup[1]+1 << "  ";
        if( _active[2] ) std::cout << "[" << _cup[2]+1 << "] "; else std::cout << " " << _cup[2]+1 << "  ";
        if( _active[3] ) std::cout << "[" << _cup[3]+1 << "] "; else std::cout << " " << _cup[3]+1 << "  ";
        if( _active[4] ) std::cout << "[" << _cup[4]+1 << "] "; else std::cout << " " << _cup[4]+1 << "  ";
        if( _active[5] ) std::cout << "[" << _cup[5]+1 << "]" << std::endl; else std::cout << " " << _cup[5]+1 << "  " << std::endl;
        std::cout << "      \n" << std::endl;
    }
}

