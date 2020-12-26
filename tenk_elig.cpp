#include "tenk_elig.h"

auto TenKElig::begin() -> decltype( _elig.begin() )
{
    return _elig.begin();
}
auto TenKElig::end() -> decltype( _elig.end() )
{
    return _elig.end();
}


/// ----------------------------------------------------------------------------
/// Eligibility implementation
auto
TenKElig::stimulate(rl::State const& afterstate)
-> void
{
    // : cast to implemented type
    auto const& as = dynamic_cast<Traced_t const&>(afterstate);

    // : increment e-trace by 1
    auto search = _elig.find(as);
    if( search == _elig.end() )
    {
        // - nonexistent
        _elig.insert( {as, 1.0} );
    }
    else
    {
        ++ search->second;
    }
}

auto
TenKElig::decay()
-> void
{
    for(auto& e : _elig )
        e.second *= _gamma*_lambda;
}

auto
TenKElig::visit(rl::State const& afterstate)
-> void
{
    decay();
    stimulate(afterstate);
}

auto
TenKElig::init()
-> void
{
    _elig.clear();
}

TenKElig::TenKElig(double gam, double lam) : _gamma (gam), _lambda(lam)
{

}
