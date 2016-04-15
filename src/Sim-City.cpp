// Sim City.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <ctime>

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
    myResolution.createExample();

    unsigned nb_usables= myResolution.evaluateTotalUsable();
    clog << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;

    time_t evaluation_startTime;
    evaluation_startTime = time(NULL);

//    float total_ratio= myResolution.evaluateRatio();
//    cout << "Ratio total : "<< total_ratio<< endl;

    time_t stopTime;
    stopTime = time(NULL);
    time_t elapsedTime = stopTime - evaluation_startTime;

    printf("The number elapsed seconds is %ld\n",elapsedTime);

    srand(time(NULL));
//    myField.generateInsAndOuts(2);
//    myField.show_ins_and_outs();



/**	Fin tests	**/

    cout << "FIN" << endl;

    return 0;
}

