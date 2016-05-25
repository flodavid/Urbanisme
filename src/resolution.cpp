#include "resolution.h"

#include <ctime>
#include <fstream>

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    params(serveDistance, roadsWidth), localSearch(new Field(nbCols, nbRows), &params), nbCells(nbCols * nbRows)
{
    Field& initField= localSearch.get_field();
    for (const Coordinates& in_out : ins_outs){
        if ( !initField.tryAdd_in_out(in_out)){
            cerr << "Les coordonnées "<< in_out<< " ne représentent pas une entrée/sortie valide"<< endl;
        }
    }
}

Resolution::Resolution(const Field &field, const Parameters &_params):
    params(_params), localSearch(new Field(field), &_params), nbCells(field.get_width()* field.get_height())
{

}

Resolution::~Resolution()
{
//    for (const Evaluation& eval : pareto_evals) {
//        delete eval;
//    }
}

//@}
/// ############################
///      Evaluation et Pareto
/// ############################
//@{

void Resolution::evaluateBothObjectives()
{
    Evaluation& myEvaluation(localSearch.get_evaluation());

    unsigned nb_usables= myEvaluation.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables : "<< nb_usables<< endl;

    // === LANCEMENT DES ALGOS D'EVALUATION ET DE RECHERCHE LOCALE === //
    cout << "Evaluation..."<< endl;
    time_t startTime, stopTime; startTime = time(NULL);

    // Evaluation
//    if (!myEvaluation.road_distances_are_initiated) {
        myEvaluation.initRoadDistances();
//    }

    stopTime = time(NULL); time_t elapsedTimeInit = stopTime - startTime; startTime = time(NULL);

    // Calcul de la moyenne des ratios
    float avg_ratio= myEvaluation.evaluateRatio();

    stopTime = time(NULL); time_t elapsedTimeEval = stopTime - startTime;

    cout << endl;
    // AFFICHAGE DES RESULTATS
    printf("\nLe nombre de secondes écoulées pour l'initialisation est %ld\n",elapsedTimeInit);

    printf("\nLe nombre de secondes écoulées pour l'évaluation est %ld\n", elapsedTimeEval);
    cout << "=> Moyenne des ratios : "<< avg_ratio<< endl<< endl;

    if (spread(myEvaluation) > 0){
        pareto_evals.push_back(myEvaluation);
    }
}

int Resolution::spread(const Evaluation& eval)
{
    int nb_deleted= 0;
    #if DEBUG_PARETO
    cout << "Propagation de la solution dominante"<< endl;
    #endif
    for (list<Evaluation>::iterator it(pareto_evals.end()); it != pareto_evals.begin(); --it)
    {
        if( (*it).is_dominated(eval) ) {
            ++nb_deleted;
            #if DEBUG_PARETO
            cout << "Suppression de l'élement à la place "<< it._M_node<< " des non dominés"<< endl;
            #endif
            pareto_evals.erase(it);
        }
    }

    return nb_deleted;
}

//@}
/// #########################
///      Recherches locales
/// #########################
//@{

Field &Resolution::localSearchUsableObjective(unsigned maxRoadsToAdd)
{
    unsigned road_num= 1;
    int gain;
    if (maxRoadsToAdd == 0) {
        maxRoadsToAdd= UNSIGNED_INFINITY;
    }
    do {
        gain= localSearch.addRoadUsable();
#if DEBUG_ADD_USABLE_ROAD
        cout << endl<< "=== Ajout de la route "<< road_num<< endl;
#endif
        ++ road_num;
    } while(gain >= 0 && road_num <= maxRoadsToAdd);

    cout << road_num<< " ajoutées"<< endl;
    cout << endl<< "===== Evaluation après maximisation du nombre d'exploitables ====="<< endl;
    evaluateBothObjectives();

    return localSearch.get_field();
}

#define MIN_PERCENT_GAIN 5.0 // TODO supprimer du calcul de gain min pour pouvoir le supprimer
Field &Resolution::localSearchAccessObjective(unsigned maxPathsToAdd)
{
    float percent_gain;
    float gain_min;
    do {
        float access_before= localSearch.get_evaluation().get_avgAccess();
//        unsigned usables_before= localSearch.get_evaluation().get_nbUsables();

        float gain_access= localSearch.addRoadsAccess(2 * params.get_serve_distance());
        percent_gain= (gain_access / access_before) * 100.0;
        cout << "Gain de "<< percent_gain<< "%"<< endl;

        float percent_usables_after= (localSearch.get_evaluation().get_nbUsables() *100.0) / (float)nbCells;
        cout << "Exploitables : "<< percent_usables_after<< "%"<< endl;
        float percent_unusable= 100.0 - percent_usables_after;
        gain_min= MIN_PERCENT_GAIN * (percent_unusable / 30.0);
        cout << "(Gain min : "<< gain_min<< ")"<< endl;
    } while ( percent_gain >= gain_min);

    cout << endl<< "===== Evaluation après maximisation de l'accessibilité ====="<< endl;
    evaluateBothObjectives();

    return localSearch.get_field();
}

//@}
/// ############################
///         Resolution
/// ############################
//@{

Field& Resolution::initResolution()
{
//    srand(time(NULL));
    localSearch.initSolution();

    return localSearch.get_field();
}

//@}
/// ##########################
///         Export
/// ##########################
//@{
bool Resolution::trySaveParetoToTxt(string fileName) const
{
    clog << "Enregistrement dans le fichier "<< fileName<< endl;
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement Pareto500" << endl;
        return false;
    }else{
        cout << "Ecriture de "<< pareto_evals.size()<< " dans un fichier lisible par GNUplot"<< endl;
        for(const Evaluation& eval : pareto_evals){
            file << eval.get_nbUsables()<< " " << eval.get_avgAccess() << endl;
        }
        file.close();
    }
    return true;
}

//@}
