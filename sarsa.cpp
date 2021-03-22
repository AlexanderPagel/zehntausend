#include "sarsa.h"

#include <cmath>

#include "randomness.h"

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
    // no special case for ending afterstates because the environment decides
    // on whether to follow with a terminating state or not. this knowledge
    // must not coded into the learning agent

    auto i = _afterstateValueTable.find( as );

    if( i == _afterstateValueTable.end() )
    {
        auto put = _afterstateValueTable.insert( {as, 500} ); /// TODO Use variable
        assert(put.second); // Not found previously so must be inserted anew
        i = put.first;
    }

    return i->second;
}

auto
Sarsa::_afterstateValueUpdate(Afterstate_t const& as) const -> double&
{
    return _afterstateValueTable.at(as);
}

auto
Sarsa::greedy(State_t const& s, bool v) const -> Action_t const&
{

//    if( s.isTerminal() )
//    {
//        return Action_t::makeNone();
//    }

    // : compute all legal actions
    ActionVector_t const& legalActions( _legalActionsLookup(s) );    /// \bc check for move semantics?

    assert(!std::empty(legalActions));
    {
        // : max over all legal actions
        auto bestAction = std::crend(legalActions);
        double maxEstimate = std::numeric_limits<double>::lowest();

        // As a bit of a hack we loop backwards (best actions are generated
        // last). Not technically required nor forbidden.
        for (auto it = std::crbegin(legalActions); it != std::crend(legalActions); ++it)
//        for( Action_t const& a : legalActions )
        {
            auto const& a = *it;
            Afterstate_t as(s, a);
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


            if( afterstateEstimate > maxEstimate )
            {
//                if( v ) std::cout << std::setw(3) << "*";

                bestAction = it;
                maxEstimate = afterstateEstimate;
            }

//            if( v ) std::cout << std::endl;
        }
//            if( v ) std::cout << std::endl << "[ENTER]" << std::endl;
//            if( v ) std::cin.ignore();
//        if (bestAction == std::crend(legalActions))
//        {
            // Unreachable unless some return estimate is double::lowest
//            assert(false);
//            return Action_t::makeNone();
//        }
        return *bestAction;
    }
//    else
//    {
//        assert(false);
//        return Action_t::makeNone();
//    }
}


auto
Sarsa::eGreedy(State_t const& s) const -> Action_t const&
{
    /// \tbc this is the case when S_ is already seen as terminating but the algorithm needs a dummy action A_
//    if( s.isTerminal() )
//    {
//        return Action_t::makeNone();
//    }

    // TODO speed difference=
    if (randomness::epsilonRandom(epsilon))
//    if( (double)std::rand()/RAND_MAX < epsilon )
    {
        auto const& legalActions = _legalActionsLookup(s);

        assert(!legalActions.empty());
        return *randomness::pickUniformRandom(legalActions);
//        return legalActions.at( rand() % legalActions.size() ); // note: should be sufficiently even distributed even when not perfect
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

            Afterstate_t as (s , a );
            Afterstate_t as_(s_, a_);

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
