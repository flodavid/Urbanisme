#include "resolution.h"

#include <ctime>

using namespace std;

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

void Resolution::evaluateBothObjectives(Evaluation& myEvaluation) const
{
    unsigned nb_usables= myEvaluation.evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables au début : "<< nb_usables<< endl;

    // === LANCEMENT DES ALGOS D'EVALUATION ET DE RECHERCHE LOCALE === //
    cout << "Evaluation..."<< endl;
    time_t startTime, stopTime; startTime = time(NULL);

    // Evaluation
    myEvaluation.initNeighbourhoodManhattan();

    stopTime = time(NULL); time_t elapsedTimeInit = stopTime - startTime; startTime = time(NULL);

    // Calcul de la moyenne des ratios
    float avg_ratio= myEvaluation.evaluateRatio();

    stopTime = time(NULL); time_t elapsedTimeEval = stopTime - startTime;

    cout << endl;
    // AFFICHAGE DES RESULTATS
    printf("\nLe nombre de secondes écoulées pour l'initialisation est %ld\n",elapsedTimeInit);

    printf("\nLe nombre de secondes écoulées pour l'évaluation est %ld\n", elapsedTimeEval);
    cout << "=> Moyenne des ratios : "<< avg_ratio<< endl<< endl;
}


bool Resolution::oneRoadUsableObjective(const LocalSearch& localSearch){
    if (localSearch.addRoadUsable()){
        return true;
    }
    else {
        return false;
    }
//        sleep(1);
//        std::this_thread::sleep_for (std::chrono::seconds(1));
}

void Resolution::localSearchUsableObjective(const LocalSearch& localSearch)
{
    unsigned road_num= 1;
    while(oneRoadUsableObjective(localSearch)) {
        cout << endl<< "=== Ajout de la route "<< road_num<< endl;
        ++ road_num;
    }
}
