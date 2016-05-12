#include "resolution.h"

#include <ctime>

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth):
    nb_cols(nbCols), nb_rows(nbRows), params(serveDistance, roadsWidth)
{

}


Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    nb_cols(nbCols), nb_rows(nbRows), params(serveDistance, roadsWidth)
{
    
}

Resolution::Resolution(const Field &field, const Parameters &_params):
    nb_cols(field.get_width()), nb_rows(field.get_height()), params(_params)
{

}

//@}
/// ############################
///      Evaluation et Pareto
/// ############################
//@{

void Resolution::evaluateBothObjectives(Evaluation& myEvaluation)
{
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
