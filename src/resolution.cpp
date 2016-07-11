#include "resolution.h"

#include <ctime>
#include <sstream>
#include <QtWidgets/QErrorMessage> /// TODO Supprimer de cette classe et faire apparaître la fenêtre ailleurs

#include "gnuplot-cpp/gnuplot_i.hpp"
#include "fieldevaluation.h"

#define PARETO_FOLDER std::string("../ParetoResults/")

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

Resolution::Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs):
    params(serveDistance, roadsWidth), localSearch(new Field(nbCols, nbRows), &params), nbCells(nbCols * nbRows)
{
    Field* initField= localSearch.get_fieldEvaluation();
    for (const Coordinates& in_out : ins_outs) {
        if ( !initField->tryAdd_in_out(in_out)) {
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
        nbCells= field_copy->get_height() * field_copy->get_width();

        pareto_evals.clear();
        emptyEvaluationsFile();
    } else {
        cout << "LA SURFACE A ETE REINITIALISEE, je recopie la nouvelle, mais conserve les solutions trouvées"<< endl;
        /// @see la copie est vraiment utile ? (elle est donnée à localsearch)
        Field* field_copy= new Field(*_field);

        // Redéfinition des valeurs
        localSearch.setField(field_copy);
    }
}

ofstream* Resolution::openEvaluationsFile(string filename_end) const
{
    ofstream* file= new ofstream;
    ostringstream oss;
    oss<< localSearch.get_field_width()<<"_"<< localSearch.get_field_height()<< "_"<< pareto_evals.size()<<"sol"<< filename_end<<".evaluations.txt";
    file->open(PARETO_FOLDER + oss.str());

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

    // === LANCEMENT DES ALGOS D'EVALUATION === //
#if LOGS_PARETO
#endif
    time_t startTime, stopTime; startTime = time(NULL);

    // Evaluation
//    if (!myEvaluation.road_distances_are_initiated) {
        myEvaluation->initRoadDistances();
//    }

    stopTime = time(NULL); time_t elapsedTimeInit = stopTime - startTime; startTime = time(NULL);

    // Calcul de la moyenne des ratios
    clog << "Evaluation..."<< endl;
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
    auto it(pareto_evals.end());
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
//    pareto_evals.remove_if();
    list<FieldEvaluation>::iterator it(pareto_evals.end());
    do {
        --it;
        if( it->is_dominated(eval) ) {
            writeDominatedEvaluation(*it);
            ++nb_deleted;
#if LOGS_PARETO
            clog << "Suppression de l'élement d'éval ("<< eval.get_nbUsables()<< ";"<< eval.get_avgAccess()<< ") des non dominés"<< endl;
#endif
            it= pareto_evals.erase(it); /// TODO pourquoi ça peut provoquer un seg fault ?
        }
    } while ( it != pareto_evals.begin() );
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
        // Evaluation de la surface après avoir lancé l'algo
        evaluateBothObjectives();
    } while(gain >= 0 && road_num <= maxRoadsToAdd);

    cout << road_num<< " ajoutées"<< endl;
    cout << endl<< "===== Evaluation après maximisation du nombre d'exploitables ====="<< endl;
    evaluateBothObjectives();

    return localSearch.get_fieldEvaluation();
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
        changeWorkField(localSearch.get_fieldEvaluation(), true);
    } while ( num_path <= maxPathsToAdd && gain_access >= 0.0);
    if (gain_access < 0.0) cout << "On a arrêté l'ajout de chemins car le gain était négatif"<< endl;

    cout << endl<< "===== Evaluation après maximisation de l'accessibilité ====="<< endl;
    evaluateBothObjectives();

    return localSearch.get_fieldEvaluation();
}

//@}
/// ############################
///         Resolution
/// ############################
//@{

FieldEvaluation* Resolution::initResolution()
{
    srand(time(NULL));
    if (!localSearch.tryInitSolution()) {
        /// TODO plus tard : enlever l'affichage de la fenêtre d'erreur de la classe Resolution
        QErrorMessage* error_window= new QErrorMessage();
        error_window->setWindowTitle("Erreur d'initialisation");
        error_window->showMessage( "Impossible d'initialiser une route si il n'y a pas au moins deux entrées/sorties" );
    }

    return localSearch.get_fieldEvaluation();
}

//@}
/// #########################
///         Export
/// #########################
//@{

void Resolution::writeDominatedEvaluation(const Evaluation &eval)
{
    evaluations_stream << eval.get_nbUsables()<< " " << eval.get_avgAccess() << endl;
}

bool Resolution::trySaveParetoToTxt(string fileName) const
{
    system("echo %cd%");

    clog << "Enregistrement dans le fichier "<< fileName<< endl;
    ofstream file(PARETO_FOLDER + fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement Pareto500" << endl;
        return false;
    }else{
        cout << "Ecriture de "<< pareto_evals.size()<< " dans un fichier lisible par GNUplot"<< endl;

        unsigned ind_eval= 0;
        for(const Evaluation& eval : pareto_evals){
            file << eval.get_nbUsables()<< " " << eval.get_avgAccess() << " " << ++ind_eval << endl;
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

std::string Resolution::drawParetoJpeg(string dataName) const
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if (Gnuplot::set_GNUPlotPath("D:/Apps/gnuplot/bin")) {
        std::cerr << "gnuplotPath a été défini"<< endl;
    } else {
        std::cerr << "gnuplotPath n'a pas pu être défini"<< endl;
    }
#endif

    try {
        string paretoInputName (PARETO_FOLDER + dataName +".pareto.txt");
        string evalsInputName  (PARETO_FOLDER + dataName +".evaluations.txt");
        string outputName      (PARETO_FOLDER + "resolutionPareto" + dataName +".jpg");

        Gnuplot gp;
        gp.set_terminal_std("jpeg");

        ostringstream title;
        title << "Front Pareto, pour une surface de taille " <<  localSearch.get_field_width()<<" par "<< localSearch.get_field_height();
        ostringstream subtitle;
        subtitle << get_nb_not_dominated() << " solution(s) non dominée(s)";
        gp.set_title(title.str() + "\\n" + subtitle.str());

        gp.set_xlabel("Nombre d'exploitables");
        gp.set_ylabel("Accessibilité moyenne");

            std::clog << "GNUPlot : "<< "set output '"+ outputName +"'"<< endl;
        gp.cmd("set output '"+ outputName +"'");
        string plot("plot '" + paretoInputName  +"' with labels font \"arial,11\" tc lt 2 lc rgb 'red', '"+ evalsInputName +"' lc rgb 'black'");
            std::clog << "GNUPlot : "<< plot << endl;
        gp.cmd(plot);

        return outputName;
    }
    catch (GnuplotException& ge)
    {
        cout << "ERROR : " << ge.what() << endl;
        return "";
    }
}

FieldEvaluation* Resolution::trySelectSavedField(unsigned index)
{
    if (index < pareto_evals.size()) {
        list<FieldEvaluation>::iterator it= pareto_evals.begin();
        for (unsigned i= 0; i < index; ++i) ++it;

        changeWorkField(new FieldEvaluation(*it, params), false);
        return localSearch.get_fieldEvaluation();
    } else return nullptr;
}

//@}
