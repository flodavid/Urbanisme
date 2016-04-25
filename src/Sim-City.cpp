// Sim City.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <ctime>

#include "stdafx.h"
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Resolution.h"

using namespace std;

/**
 * Création du terrain de l'exemple, exécution de l'évaluation, affichage du résultat,
 * affichage tu temps pris par de l'évaluation
 */
int main()
{
    Field myField(5, 5);
    Parameters myParameters(1, 2);

    Resolution myResolution(myField, myParameters);

/**		Tests		**/
    myResolution.createExample();

    time_t startTime;
    time_t stopTime;
    time_t elapsedTime;
    
    unsigned nb_usables= myResolution.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;
    
    startTime = time(NULL);
    myResolution.initNeighbourhoodManhattan();
    stopTime = time(NULL);
    elapsedTime = stopTime - startTime;
    printf("Le nombre de secondes écoulées pour l'initialisation est %ld\n",elapsedTime);
    
    startTime = time(NULL);

    float avg_ratio= myResolution.evaluateRatio(nb_usables);
//     float total_ratio= myResolution.threadsEvaluateRatio();
    stopTime = time(NULL);
    elapsedTime = stopTime - startTime;
    
    cout << "Moyenne des ratios : "<< avg_ratio<< endl;
    printf("Le nombre de secondes écoulées pour l'évaluation est %ld\n",elapsedTime);

    srand(time(NULL));
//    myField.generateInsAndOuts(2);
//    myField.show_ins_and_outs();

/**	Fin tests	**/

    cout << "FIN" << endl;

    return 0;
}

