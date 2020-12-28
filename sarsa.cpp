#include "sarsa.hpp"

#include <cmath>

Sarsa::ActionVector_t const&
Sarsa::_legalActionsLookup(State_t const& s) const
{
    // : search for previously computed legal moves
    auto i = _legalActionsTable.find( s );

    if( i == _legalActionsTable.end() )
    {
        // - element missing
        // : add the required element
        /// \tbc check if the overloaded insert function taking rvalue ref is called
        auto put = _legalActionsTable.insert( {s, Game_t::legalActions(s)} );

        if( put.second == false )
            exit(7687);
        i = put.first;
    }

    return i->second;
}

auto
Sarsa::_afterstateValueLookup(Afterstate_t const& as) const -> double
{

    // : finishing means 0
    if( as.fromTerminal() )
        return 0;

    /// \tbc the following correct? :
    // no special case for ending afterstates because the environment decides on whether to follow with a terminating state or not.
    // this knowledge must not coded into the learning agent

    Afterstate_t transf( as );

    // : transform
    /// \tbc ther should be a function pointer member to do this transformation
//    if( transf.points() > 2000 )
//        transf.points() = 2000;

    auto i = _afterstateValueTable.find( transf );

    if( i == _afterstateValueTable.end() )
    {
        auto put = _afterstateValueTable.insert( {transf, 500} ); /// \bc make variable
        if( put.second == false )
            exit(43258);
        i = put.first;
    }

    return i->second;
}

auto
Sarsa::_afterstateValueUpdate(Afterstate_t const& as) const -> double&
{
    Afterstate_t transf( as );
    return _afterstateValueTable.at( transf );
}

auto
Sarsa::greedy(State_t const& s, bool v) const -> Action_t
{

    if( s.terminal() )
    {
        return Action_t::zero();
    }

    // : compute all legal actions
    ActionVector_t const& legalActions( _legalActionsLookup(s) );    /// \bc check for move semantics?

    if( !legalActions.empty() )
    {
        // : max over all legal actions
        Action_t bestAction( Action_t::zero() );  /// \write std ctor and remore init;
        double maxEstimate = std::numeric_limits<double>::lowest();

        for( Action_t const& a : legalActions )
        {
            Afterstate_t as( _game_t::afterstate(s, a) );                                          // not: only allows legal afterstates (i hope)
//            double reward = Environment_t::simulate(s, a).first;
            double afterstateEstimate ( _afterstateValueLookup(as) );      /// \bc write lookup function that checks if existing and else throws error or exits or sth
//            double sum = reward + gamma*afterstateEstimate;
            if( v )
            {
                cout << "Action( ";
                for(int i=0;i<6;++i) cout << a.putAside()[i];
                cout << " " << a.putAside()[6]
                     << " | " << a.finishes() << " ) --> "
                     << setprecision(2) << std::fixed << afterstateEstimate;
            }


            if( afterstateEstimate >= maxEstimate )
            {
                if( v ) cout << setw(3) << "*";

                bestAction = a;
                maxEstimate = afterstateEstimate;
            }

            if( v ) cout << endl;
        }
            if( v ) cout << endl << "[ENTER]" << endl;
            if( v ) cin.ignore();
        return bestAction;
    }
    else
    {
        return Action_t::zero();
    }
}


auto
Sarsa::eGreedy(State_t const& s) const -> Action_t
{
    /// \tbc this is the case when S_ is already seen as terminating but the algorithm needs a dummy action A_
    if( s.terminal() )
    {
        return Action_t::zero();
    }

    if( (double)std::rand()/RAND_MAX < epsilon )
    {
        auto legalActions = std::move( _legalActionsLookup(s) );

        if( !legalActions.empty() )
        {
            return legalActions.at( rand() % legalActions.size() ); // note: should be sufficiently even distributed even when not perfect
        }
        else
        {
            return Action_t::zero();
        }
    }
    else
    {
        return greedy( s );
    }
}

auto
Sarsa::performLearningEpisodes(unsigned int n, unsigned int l, std::ostream& os) & -> void
{
    unsigned int const t = n;

    for( ; n != 0; --n )
    {
        auto s( State_t::randomStart() ); /// \tbc moves?
//            if(
//                s.thrown()[0] == 1 &&
//                s.thrown()[1] < 3 &&
//                s.thrown()[2] < 3 &&
//                s.thrown()[3] < 3 &&
//                s.thrown()[4] == 1 &&
//                s.thrown()[5] < 3 &&
//                s.thrown()[6] == 6 &&
//                a.putAside()[0] > 0 )
//                    cout << setw(0);

        do /// \note   what if s is terminal for other games?
        {
            auto a( eGreedy(s) );
            auto simulEnvFeedback = ( Environment_t::simulate(s, a) );    /// \tbc decltype or sth all these things to know what they are? :D
            /// \tbc make reward be computed by the simulation, not the player :D
            // reward = gain in points

            auto r  = simulEnvFeedback.first;
            auto s_ = simulEnvFeedback.second;


            auto a_( greedy(s_) );

            Afterstate_t as ( _game_t::afterstate(s,a) );
            Afterstate_t as_ ( _game_t::afterstate(s_,a_) );

//            if( as.diceLeft() == 0 && as.points() == 350 && as.fromTerminal() == false )
//                cout << "appearance. why does this have an estimate of 605??" << endl;


            double prev = _afterstateValueLookup( as );
            double nextEst = _afterstateValueLookup( as_ );

//            if(
//               s.thrown()[0] == 0&&
//               s.thrown()[1] == 0&&
//               s.thrown()[2] == 1&&
//               s.thrown()[3] == 1&&
//               s.thrown()[4] == 3&&
//               s.thrown()[5] == 0&&
//               s.points() == 100
//               )
//                cout << setw(0);

            _afterstateValueUpdate( as ) = prev + alpha*(r+gamma*nextEst - prev);

            s = s_;
            a = a_;
        }
        while( !s.terminal() );

        if( l != 0 )
            if( t != 0 )
                if( n % (int)(t/l+1) == 0 ) /// \tbc get an exact loading function?
                    os << "=";
    }

    return;

}


Sarsa::Sarsa(double alph, double epsil, double gam) noexcept
    : alpha{alph}, epsilon{epsil}, gamma{gam}, _afterstateValueTable{}, _legalActionsTable()
{
    if( 1/RAND_MAX > epsilon )
        exit(36762);

}

