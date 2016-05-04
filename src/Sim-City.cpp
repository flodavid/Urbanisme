// Sim City.cpp : Defines the entry point for the console application.

#include <iostream>
#include <ctime>

#include <QApplication>

#include "stdafx.h"
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Display/fieldview.hpp"
#include "evaluation.h"
#include "localsearch.h"

using namespace std;

void evaluateBothObjectives(Evaluation& myEvaluation)
{

    unsigned nb_usables= myEvaluation.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;

    // === LANCEMENT DES ALGOS D'EVALUATION ET DE RECHERCHE LOCALE === //
    time_t startTime, stopTime; startTime = time(NULL);

    // Evaluation
    myEvaluation.initNeighbourhoodManhattan();

    stopTime = time(NULL); time_t elapsedTimeInit = stopTime - startTime; startTime = time(NULL);

    // Calcul de la moyenne des ratios
    float avg_ratio= myEvaluation.evaluateRatio();

    stopTime = time(NULL); time_t elapsedTimeEval = stopTime - startTime;


    // AFFICHAGE DES RESULTATS
    printf("\nLe nombre de secondes écoulées pour l'initialisation est %ld\n",elapsedTimeInit);

    printf("\nLe nombre de secondes écoulées pour l'évaluation est %ld\n", elapsedTimeEval);
    cout << "=> Moyenne des ratios : "<< avg_ratio<< endl<< endl;
}

/*! \mainpage Page principale de la documentation du projet "Urbanisme"
 * Github : https://github.com/flodavid/Urbanisme
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
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
    
    // Paramètres du problèmes
    Parameters myParameters(2, 1);

    Field myField(20, 20);
//@{
//     // Solution de l'exemple :
//    myField.createExample(myParameters.get_serve_distance());
//@}
    
//@{
    // Angle
        myField.add_in_out(19,4);
        myField.add_in_out(11,19);
    // Mm colonne
//        myField.add_in_out(0,4);
//        myField.add_in_out(0,19);
    // En face
//        myField.add_in_out(0,4);
//        myField.add_in_out(19,8);
    // E/S exemple
//        myField.add_in_out(9,19);
//        myField.add_in_out(9,0);
//@}

    // Solution avec recherche locale
    LocalSearch myLocalSearch(&myField, &myParameters);
    myLocalSearch.initSolution();

    // Parcelles utilisables
    Evaluation myEvaluation(myField, myParameters);
    cout << endl<< "===== Evaluation avant recherche locale ====="<< endl;
    evaluateBothObjectives(myEvaluation);

    /** Tests **/
    for (unsigned road_num= 1; road_num < 83; ++road_num) {
        cout << endl<< "=== Ajout de la route "<< road_num<< endl;
        myLocalSearch.addRoad();
    }

    /** Fin tests **/

    myField.updateUsables(myParameters.get_serve_distance());

    // Fenêtre
    FieldWidget* myFieldWidget= new FieldWidget(&(myField));
    myFieldWidget->redraw();
    myFieldWidget->show();

    cout << endl<< "===== Evaluation après recherche locale ====="<< endl;
    evaluateBothObjectives(myEvaluation);

    return app->exec();
}

