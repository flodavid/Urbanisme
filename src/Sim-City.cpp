// Sim City.cpp : Defines the entry point for the console application.

#include <iostream>
#include <ctime>

#include <QApplication>

#include "stdafx.h"
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Display/fieldview.hpp"
#include "Resolution.h"

using namespace std;

/*! \mainpage Page principale de la documentation du projet "Urbanisme" 
 * \section intro_sec Introduction 
 * Ce projet à pour but d'optimiser le placement de routes sur une surface, en maximisant les zones exploitables (desservies par une route) et leur accessibilité.
 * On a donc un paramètre de distance de desserte défini au début de l'exécution.
 *
 * \section install_sec Installation 
 * Le projet nécessite un compilateur C++11 (g++ 4.8 minimum par exemple).
 * Utiliser cmake et make pour compiler.
 * Pour compiler les tests, la librairie CPP Unit doit être correctement installée sur la machine 

 * \subsection step1 Etape 1: Modélisation d'une solution 
 * Les classes du dossier "Engine" permettent de modéliser une solution et d'effectuer des modifications simples sur celle-ci 

 * \subsection step2 Etape 2 : Evaluation d'une solution
 * La classe Resolution est chargée d'effectuer l'évaluation et le stockage des données calculée lors de celle-ci, ainsi que de mettre à jour ces valeurs lors des modifications de la solution.
 *
 * \subsection step3 Etape 3: Définition de méthodes de création d'un voisin d'une solution
 * A faire.
 * 
 * \subsection step4 Etape 4: Affichage d'une solution
 * A faire, utilisation de Qt.
 */
/**
 * Création du terrain de l'exemple, exécution de l'évaluation, affichage du résultat,
 * affichage tu temps pris par de l'évaluation
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Field myField(5, 5);
    Parameters myParameters(1, 2);

    Resolution myResolution(myField, myParameters);
    FieldWidget* myFieldWidget= new FieldWidget(&(myResolution.field));

/**		Tests		**/
    myResolution.createExample();
    myFieldWidget->redraw();
    myFieldWidget->show();

    time_t startTime;
    time_t stopTime;
    
    unsigned nb_usables= myResolution.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;
    
    startTime = time(NULL);
    myResolution.initNeighbourhoodManhattan();
    stopTime = time(NULL);
    time_t elapsedTimeInit = stopTime - startTime;
    
    startTime = time(NULL);

    float avg_ratio= myResolution.evaluateRatio(nb_usables);
//     float total_ratio= myResolution.threadsEvaluateRatio();
    stopTime = time(NULL);
    time_t elapsedTimeEval = stopTime - startTime;

    printf("Le nombre de secondes écoulées pour l'initialisation est %ld\n",elapsedTimeInit);
    cout << "Moyenne des ratios : "<< avg_ratio<< endl;
    printf("Le nombre de secondes écoulées pour l'évaluation est %ld\n", elapsedTimeEval);

    srand(time(NULL));

/**	Fin tests	**/

    cout << "FIN" << endl;

    return app.exec();
}

