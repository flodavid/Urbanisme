#include "resolution.h"

#include <ctime>
#include <sstream>
#include <QtWidgets/QErrorMessage> /// TODO Supprimer de cette classe et faire apparaître la fenêtre ailleurs

#include "fieldevaluation.h"

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    params(serveDistance, roadsWidth), localSearch(new Field(nbCols, nbRows), &params), nbCells(nbCols * nbRows)
{
    Field& initField= localSearch.get_field();
    for (const Coordinates& in_out : ins_outs) {
        if ( !initField.tryAdd_in_out(in_out)) {
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
    ofstream& eval_file= *(openEvaluationsFile("_saved"));
    eval_file << evaluations_stream;
    eval_file.close();
    delete &eval_file;

//    for (const Evaluation& eval : pareto_evals) {
//        delete eval;
//    }
}

//@}
/// ##################
///      Setters
/// ##################
//@{

void Resolution::changeWorkField(Field *_field, bool newField)
{
    if (newField) {
        cout << "LA SURFACE A CHANGEE, je recopie la nouvelle et efface solutions précédemment trouvées"<< endl;
        /// TODO demander à l'utilisateur si il veut sauvegarder les résultats avant d'effacer le champ
        /// @see la copie est vraiment utile ? (elle est donnée à localsearch)
        Field* field_copy= new Field(*_field);

        // Redéfinition des valeurs
        localSearch.setField(field_copy);
        pareto_evals.clear();
        nbCells= field_copy->get_height() * field_copy->get_width();

        emptyEvaluationsFile();
    } else {
        cout << "Surface inchangée"<< endl;
    }
}

ofstream* Resolution::openEvaluationsFile(string filename_end)
{
    ofstream* file= new ofstream;
    ostringstream oss;
    oss<< localSearch.get_field().get_width()<<"_"<< localSearch.get_field().get_height()<< "_"<< pareto_evals.size()<<"sol"<< filename_end<<".evaluations.txt";
    file->open(oss.str());

    return file;
}

void Resolution::emptyEvaluationsFile()
{
    evaluations_stream.flush();
}

//@}
/// ############################
///      Evaluation et Pareto
/// ############################
//@{
void Resolution::evaluateBothObjectives()
{
    FieldEvaluation* myEvaluation(localSearch.get_fieldEvaluation());

    unsigned nb_usables= myEvaluation->evaluateTotalUsable();
    cout << "Nombre total de parcelles exploitables : "<< nb_usables<< endl;

    // === LANCEMENT DES ALGOS D'EVALUATION ET DE RECHERCHE LOCALE === //
#if LOGS_PARETO
    clog << "Evaluation..."<< endl;
#endif
    time_t startTime, stopTime; startTime = time(NULL);

    // Evaluation
//    if (!myEvaluation.road_distances_are_initiated) {
        myEvaluation->initRoadDistances();
//    }

    stopTime = time(NULL); time_t elapsedTimeInit = stopTime - startTime; startTime = time(NULL);

    // Calcul de la moyenne des ratios
    float avg_ratio= myEvaluation->evaluateRatio();

    stopTime = time(NULL); time_t elapsedTimeEval = stopTime - startTime;

    cout << endl;
    // AFFICHAGE DES RESULTATS
    clog << "Le nombre de secondes écoulées est, pour l'initialisation : "<< elapsedTimeInit
        << " pour l'évaluation : "<< elapsedTimeEval<< endl;
    cout << "=> Moyenne des ratios : "<< avg_ratio<< endl<< endl;

    if (isNotDominated(*myEvaluation)) {
#if LOGS_PARETO
        clog << "La solution n'est pas dominée, on la propage et on l'ajoute"<< endl;
#endif
        spread(*myEvaluation);
        pareto_evals.push_back(*myEvaluation);
#if LOGS_PARETO
        clog << "Il reste "<< pareto_evals.size()<< " solution non dominées"<< endl;
#endif
    } else {
#if LOGS_PARETO
        clog << "La solution est dominée (Il y "<< pareto_evals.size()<< " solution non dominées)"<< endl;
#endif
        writeDominatedEvaluation(*myEvaluation);
    }
}

bool Resolution::isNotDominated(const Evaluation &eval)
{
//    int x= pareto_evals.size();
    list<FieldEvaluation>::iterator it(pareto_evals.end());
    do {
        --it;
        if( eval.is_dominated(*it) ) {
            #if DEBUG_PARETO
            cout << "La solutions est dominée par une des précédentes solutions"<< endl;
            #endif
            return false;
        }
    } while ( it != pareto_evals.begin() );
    return true;
}

int Resolution::spread(const Evaluation& eval)
{
    int nb_deleted= 0;
#if LOGS_PARETO
    cout << "Propagation de la solution dominante"<< endl;
#endif
    for (list<FieldEvaluation>::iterator it(pareto_evals.end()); it != pareto_evals.begin(); --it)
    {
        if( it->is_dominated(eval) ) {
            ++nb_deleted;
#if LOGS_PARETO
            clog << "Suppression de l'élement d'éval ("<< eval.get_nbUsables()<< ";"<< eval.get_avgAccess()<< ") des non dominés"<< endl;
#endif
            it= pareto_evals.erase(it); /// TODO pourquoi ça peut provoquer un seg fault ?
        }
    }
#if LOGS_PARETO
    clog << nb_deleted << " supprimés dominés par la solution d'éval ("<< eval.get_nbUsables()<< ";"<< eval.get_avgAccess()<< ")" << endl;
#endif
    return nb_deleted;
}

//@}
/// #########################
///      Recherches locales
/// #########################
//@{

FieldEvaluation * Resolution::localSearchUsableObjective(unsigned maxRoadsToAdd)
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
        // Evaluation de la surface avant de lancer l'algo
        evaluateBothObjectives();
    } while(gain >= 0 && road_num <= maxRoadsToAdd);

    cout << road_num<< " ajoutées"<< endl;
    cout << endl<< "===== Evaluation après maximisation du nombre d'exploitables ====="<< endl;
    evaluateBothObjectives();

    return &(localSearch.get_field());
}

//#define MIN_PERCENT_GAIN 5.0 // TODO supprimer du calcul de gain min pour pouvoir le supprimer
FieldEvaluation *Resolution::localSearchAccessObjective(unsigned maxPathsToAdd)
{

//    float gain_min;
    unsigned num_path= 1;
    float gain_access= 0.0;
    do {
        // Evaluation de la surface avant de lancer l'algo
        evaluateBothObjectives();

        float access_before= localSearch.get_fieldEvaluation()->get_avgAccess();
//        unsigned usables_before= localSearch.get_evaluation().get_nbUsables();

        gain_access= localSearch.addRoadsAccess(2 * params.get_serve_distance());
        float percent_gain= (gain_access / access_before) * 100.0;
        cout << "Pour chemin "<< num_path<<", Gain de "<< percent_gain<< "%"<< endl;

        float percent_usables_after= (localSearch.get_fieldEvaluation()->get_nbUsables() *100.0) / (float)nbCells;
        cout << "Exploitables : "<< percent_usables_after<< "%"<< endl;

        /// @see tentative d'augmentation de l'accessibilité tant que le gain est suffisament important
        /// rajouter condition dans le while
//        float percent_unusable= 100.0 - percent_usables_after;
//        gain_min= MIN_PERCENT_GAIN * (percent_unusable / 30.0);
//        cout << "(Gain min : "<< gain_min<< ")"<< endl;

        ++num_path;
    } while ( num_path <= maxPathsToAdd && gain_access >= 0.0);
    if (gain_access < 0.0) cout << "On a arrêté l'ajout de chemins car le gain était négatif"<< endl;

    cout << endl<< "===== Evaluation après maximisation de l'accessibilité ====="<< endl;
    evaluateBothObjectives();

    return &(localSearch.get_field());
}

//@}
/// ############################
///         Resolution
/// ############################
//@{

FieldEvaluation* Resolution::initResolution()
{
//    srand(time(NULL));
    if (!localSearch.tryInitSolution()) {
        /// TODO plus tard : enlever l'affichage de la fenêtre d'erreur de la classe Resolution
        QErrorMessage* error_window= new QErrorMessage();
        error_window->setWindowTitle("Erreur d'initialisation");
        error_window->showMessage( "Impossible d'initialiser une route si il n'y a pas au moins deux entrées/sorties" );
    }

    return &(localSearch.get_field());
}

//@}
/// #########################
///         Export
/// #########################
//@{

bool Resolution::trySaveParetoToTxt(string fileName)
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

        // Ecriture dans un fichier des solutions dominées
        ofstream& eval_file= *(openEvaluationsFile());
        eval_file << evaluations_stream.str();
        eval_file.close();
        delete &eval_file;
    }
    return true;
}

void Resolution::writeDominatedEvaluation(const Evaluation &eval)
{
    evaluations_stream << eval.get_nbUsables()<< " " << eval.get_avgAccess() << endl;
}

//@}
