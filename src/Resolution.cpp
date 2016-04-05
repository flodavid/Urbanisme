#include "stdafx.h"
#include "Resolution.h"

using namespace std;

Resolution::Resolution(const Field& _field, const Parameters& _params):
	field(_field), params(_params)
{
}

Resolution::~Resolution()
{
}

// #########################
// 	Getters
// #########################

// #########################
// 	Setters
// #########################
void Resolution::setParams(const Parameters& _params)
{
    params= _params;
}


// #########################
// 	Evaluations
// #########################

unsigned int Resolution::evaluateTotalUsable() const
{
    unsigned nb_usables;
    for (State parcel_state : field){
	if (parcel_state == usable) {
	    ++nb_usables;
	}
    }
    
#if DEBUG_EVALUATION
    cout << "Total number of usables parcels : "<< nb_usables<< endl;
#endif
    
    return nb_usables;
}
