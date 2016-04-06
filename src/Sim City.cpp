// Sim City.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "stdafx.h"
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Resolution.h"

using namespace std;

int main()
{
	Field myField(5, 5);
	Parameters myParameters(1, 2);
	
	Resolution myResolution(myField, myParameters);
	
    /**		Tests		**/
	myField.generateInsAndOuts(2);
	myField.show_ins_and_outs();
	
	unsigned nb_usables= myResolution.evaluateTotalUsable();
	
#if DEBUG_EVALUATION
	clog << "Nombre total de parcels exploitables au début : "<< nb_usables<< endl;
#endif
	
	
	
    /**	Fin tests	**/

	cout << "FIN" << endl;

    return 0;
}

