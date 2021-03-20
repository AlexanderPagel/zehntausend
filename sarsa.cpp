#include "sarsa.h"

#include <cmath>

Sarsa::ActionVector_t const&
Sarsa::_legalActionsLookup(State_t const& s) const
{
    // Search for previously computed legal moves
    auto search = _legalActionsTable.find( s );

    // Compute anew and cache if not found
    if( search == _legalActionsTable.end() )
    {
        auto put = _legalActionsTable.insert( {s, Game_t::generateActions(s)} );
        assert(put.second); // Insertion took place
        search = put.first;
    }

    return search->second;
}

double
Sarsa::_afterstateValueLookup(Afterstate_t const& as) const
{
    // In terminal states there is no future reward
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

    if( s.isTerminal() )
    {
        return Action_t::makeNone();
    }

    // : compute all legal actions
    ActionVector_t const& legalActions( _legalActionsLookup(s) );    /// \bc check for move semantics?

    if( !legalActions.empty() )
    {
        // : max over all legal actions
        Action_t bestAction( Action_t::makeNone() );  /// \write std ctor and remore init;
        double maxEstimate = std::numeric_limits<double>::lowest();

        for( Action_t const& a : legalActions )
        {
            Afterstate_t as( _game_t::afterstate(s, a) );                                          // not: only allows legal afterstates (i hope)
//            double reward = Environment_t::simulate(s, a).first;
            double afterstateEstimate ( _afterstateValueLookup(as) );      /// \bc write lookup function that checks if existing and else throws error or exits or sth
//            double sum = reward + gamma*afterstateEstimate;
            if( v )
            {
//                std::cout << "Action( ";
//                for(int i=0;i<6;++i) std::cout << a.putAside()[i];
//                std::cout << " " << a.putAside()[6]
//                     << " | " << a.finishes() << " ) --> "
//                     << std::setprecision(2) << std::fixed << afterstateEstimate;
            }


            if( afterstateEstimate >= maxEstimate )
            {
                if( v ) std::cout << std::setw(3) << "*";

                bestAction = a;
                maxEstimate = afterstateEstimate;
            }

            if( v ) std::cout << std::endl;
        }
            if( v ) std::cout << std::endl << "[ENTER]" << std::endl;
            if( v ) std::cin.ignore();
        return bestAction;
    }
    else
    {
        assert(false);
        return Action_t::makeNone();
    }
}


auto
Sarsa::eGreedy(State_t const& s) const -> Action_t
{
    /// \tbc this is the case when S_ is already seen as terminating but the algorithm needs a dummy action A_
    if( s.isTerminal() )
    {
        return Action_t::makeNone();
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
            return Action_t::makeNone();
        }
    }
    else
    {
        return greedy( s );
    }
}

void
Sarsa::performLearningEpisodes(unsigned int n, unsigned int l, std::ostream& os)
{
    unsigned int const t = n;
    refac::Environment e;

    for( ; n != 0; --n )
    {
        e.restart();
        auto s = e.getState();

        while(!e.episodeFinished())
        {
            auto a( eGreedy(s) );

            auto r  = e.takeAction(a);
            auto s_ = e.getState();

            auto a_( greedy(s_) );

            Afterstate_t as ( Environment_t::afterstate(s,a) );
            Afterstate_t as_ ( Environment_t::afterstate(s_,a_) );

            double prev = _afterstateValueLookup( as );
            double nextEst = _afterstateValueLookup( as_ );

            _afterstateValueUpdate( as ) = prev + alpha*(r+gamma*nextEst - prev);

            s = s_;
        }

        // Print a loading bar so we know the algorithm didn't hung itself
        if( l != 0 )
            if( t != 0 )
                if( n % (int)(t/l+1) == 0 ) /// \tbc get an exact loading function?
                    os << "=";
    }

    return;

}


Sarsa::Sarsa(double alph, double epsil, double gam)
    : alpha{alph}, epsilon{epsil}, gamma{gam}, _afterstateValueTable{}, _legalActionsTable()
{
    if( 1/RAND_MAX > epsilon )
        exit(36762);

}
