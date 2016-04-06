#include "stdafx.h"
#include "Resolution.h"

#include <cassert>

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
    unsigned nb_usables= 0;
    for (vector<State> row_parcel_state : field) {
	for (State parcel_state : row_parcel_state) {
	    cout << "parcel_state = "<< parcel_state<<endl;
	    assert(parcel_state >= -1 &&  parcel_state<= unusable);
	    if (parcel_state == usable) {
		++nb_usables;
	    }
	}
    }
    
#if DEBUG_EVALUATION
    cout << "Total number of usables parcels : "<< nb_usables<< endl;
#endif
    assert(nb_usables >=0 && nb_usables < field.getNbParcels() && "nombre d'exploitables incohérent");
    
    return nb_usables;
}


float Resolution::evaluateTotalRatio() const
{
    float total_ratio= 0.0;
    
    
    return total_ratio;
}
