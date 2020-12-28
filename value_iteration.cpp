#include "value_iteration.h"

ValueIteration::ValueIteration() : vPi{}
{
    // define size of the hash function
    vPi.reserve(numberOfStates);

    // reset all elements to 0
    for( State s; s.isInRange(); ++s )
    {
        vPi.insert( {s, 0} );
    }

}
