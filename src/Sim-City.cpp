// Sim City.cpp : Defines the entry point for the console application.

#include <iostream>

#include <QApplication>

#include "stdafx.h"
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Display/fieldview.hpp"
#include "evaluation.h"
#include "localsearch.h"
#include "resolution.h"

using namespace std;

FieldWidget* initWindow(const Field* myField){
    FieldWidget* field_widget= new FieldWidget(myField);
    field_widget->redraw();
    field_widget->show();

    return field_widget;
}

/**
 * Création du terrain de l'exemple, exécution de l'évaluation, affichage du résultat,
 * affichage tu temps pris par de l'évaluation
 */
#include <chrono>
#include <unistd.h>
int main(int argc, char* argv[])
{
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
    

    // Paramètres du problèmes
    Parameters myParameters(2, 1);

    Field myField(20, 20);

    Resolution myResolution(myField, myParameters);
    //@{
    //     // Solution de l'exemple :
    //    myField.createExample();
    //@}
    
    //@{
    // Angle
    //        myField.add_in_out(11,19);
    //        myField.add_in_out(19,4);
    // Mm colonne
    //        myField.add_in_out(0,4);
    //        myField.add_in_out(0,19);
    // En face : coude
    //        myField.add_in_out(19,8);
    //        myField.add_in_out(0,4);
    // E/S exemple
    myField.add_in_out(9,19);
    myField.add_in_out(9,0);

    // Solution avec recherche locale
    LocalSearch myLocalSearch(&myField, &myParameters);
    myLocalSearch.initSolution();
    //@}
    
    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    myField.defineUsables(myParameters.get_serve_distance());

    // Parcelles utilisables
    Evaluation myEvaluation(myField, myParameters);
    cout << endl<< "===== Evaluation avant recherche locale ====="<< endl;
    myResolution.evaluateBothObjectives(myEvaluation);

    // Fenêtre
    FieldWidget* myFieldWidget= initWindow(&myField);

    /** Tests **/
    myResolution.localSearchUsableObjective(myLocalSearch);

    myFieldWidget->redraw();
    /** Fin tests **/

    cout << endl<< "===== Evaluation après recherche locale ====="<< endl;
    myResolution.evaluateBothObjectives(myEvaluation);

    return app->exec();
}

/*! \mainpage Page principale de la documentation du projet "Urbanisme"
 * Github : https://github.com/flodavid/Urbanisme
 * Retour au site de présentation : https://flodavid.github.io/Urbanisme
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
 * + Une méthode ajoute une seule route, afin de trouver une solution avec un nombre de parcelles exploitables supérieur ou égal
 * + En cours : Une méthode qui ajoute un nombre définit de routes afin d'obtenir une solution avec une meilleure accessibilité
 * + A faire : des méthodes qui "perturbent" la solution courantes, pour trouver sortir du minimum local
 * + A faire : stocker les résultats afin de créer un front Pareto et choisir les meilleurs (seulement ?) voisins
 *
 * \subsection step4 Etape 4: Affichage d'une solution
 * Utilisation de Qt.
 */
