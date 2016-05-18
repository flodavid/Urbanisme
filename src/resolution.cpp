#include "resolution.h"

#include <ctime>

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    params(serveDistance, roadsWidth),
    localSearch(new Field(nbCols, nbRows), &params)
{
    Field& initField= localSearch.get_evaluation()->get_field();
    for (const Coordinates& in_out : ins_outs){
        if ( !initField.tryAdd_in_out(in_out)){
            cerr << "Les coordonnées "<< in_out<< " ne représentent pas une entrée/sortie valide"<< endl;
        }
    }
}

Resolution::Resolution(const Field &field, const Parameters &_params):
    params(_params), localSearch(new Field(field), &_params)
{

}

//@}
/// ############################
///      Resolution
/// ############################
//@{

Field& Resolution::initResolution()
{
    localSearch.initSolution();

    return localSearch.get_evaluation()->get_field();
}

Field &Resolution::launchResolution()
{
    // Parcelles utilisables
    cout << endl<< "===== Evaluation avant recherche locale ====="<< endl;
    evaluateBothObjectives();

    /** Tests **/
    localSearchUsableObjective(localSearch);

    cout << endl<< "===== Evaluation après nb exploitables ====="<< endl;
    evaluateBothObjectives();

    for (unsigned i= 0; i < 3; ++i) {
       localSearch.addRoadsAccess(2 * params.get_serve_distance());
    }

    cout << endl<< "===== Evaluation après accessibilité ====="<< endl;
    evaluateBothObjectives();

    return localSearch.get_evaluation()->get_field();
}

//@}
/// ############################
///      Evaluation et Pareto
/// ############################
//@{

void Resolution::evaluateBothObjectives()
{
    Evaluation& myEvaluation= *(localSearch.get_evaluation());

    unsigned nb_usables= myEvaluation.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;

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

    if (spread(&myEvaluation) > 0){
        pareto_evals.push_back(&myEvaluation);
    }
}

int Resolution::spread(const Evaluation* eval)
{
    int nb_deleted= 0;
    #if DEBUG_PARETO
    cout << "Propagation de la solution dominante"<< endl;
    #endif
    for (list<const Evaluation*>::iterator it(pareto_evals.end()); it != pareto_evals.begin(); --it)
    {
        if( (*it)->is_dominated(*eval) ) {
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

void Resolution::localSearchUsableObjective(const LocalSearch& localSearch)
{
    unsigned road_num= 1;
    while(localSearch.addRoadUsable()) {
#if DEBUG_ADD_USABLE_ROAD
        cout << endl<< "=== Ajout de la route "<< road_num<< endl;
#endif
        ++ road_num;
    }
}
