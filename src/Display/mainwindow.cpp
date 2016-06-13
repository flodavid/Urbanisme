#include "mainwindow.hpp"

#include <ostream>

#include <QtGui/QDesktopServices>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtGui/QMouseEvent>

#include "gnuplot-cpp/gnuplot_i.hpp"

using std::cout; using std::cerr; using std::endl;

/// #########################
///      Constructeurs
/// #########################
//@{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), initialField(0,0), parameters(0,0)
{
    //    askParams();
    const QPoint* size= askSizes();
    initialField.set_width(size->x());
    initialField.set_height(size->y());
    delete size;

    fieldWidget= new FieldWidget(&initialField, parameters.get_serve_distance());

    resolution= new Resolution(initialField, parameters);

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(serveDistance, roadsWidth)
{
    fieldWidget= new FieldWidget(&initialField, serveDistance);
    initialField.setUsables(parameters.get_serve_distance());

    resolution= new Resolution(initialField, parameters);

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, const Parameters &params, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(params)
{
    fieldWidget= new FieldWidget(&initialField, params.get_serve_distance());
    initialField.setUsables(parameters.get_serve_distance());

    resolution= new Resolution(*(fieldWidget->get_field()), parameters);

    initComponents();
    initEvents();
}

MainWindow::MainWindow(const Field& field, const Parameters &params, QWidget *parent)
    : QMainWindow(parent), initialField(field), parameters(params)
{
    fieldWidget= new FieldWidget(&initialField, parameters.get_serve_distance());
    initialField.setUsables(parameters.get_serve_distance());

    resolution= new Resolution(initialField, parameters);

    initComponents();
    initEvents();
}

MainWindow::~MainWindow()
{
    if (aboutWindow == nullptr) {
        delete aboutWindow;
    }
    delete aboutAction;
    delete initAction;
    delete evalAction;
    delete usableAction;
    delete accessAction;
    delete resetAction;
    delete flushAction;
    delete exportAction;

    delete resolution;

    delete fieldWidget;
}

#define INITIAL_SCALE 32
void MainWindow::initComponents()
{
    aboutWindow= nullptr;
    initWindow= nullptr;

    resize(initialField.get_width() * INITIAL_SCALE , initialField.get_height() * INITIAL_SCALE +20);

    setCentralWidget(fieldWidget);
    fieldWidget->redraw();
    fieldWidget->show();

    // Menu bar
    QMenu* othersMenu= menuBar()->addMenu( tr("Others⇲") );
    askAction= othersMenu->addAction( tr("Ask") );
    aboutAction= othersMenu->addAction( tr("About") );
    initAction= menuBar()->addAction( tr("Initialise") );
    evalAction= menuBar()->addAction( tr("Evaluate") );
    QMenu* menu_resols= menuBar()->addMenu( tr("Resolutions⇲") );
    usableAction= menu_resols->addAction( tr("Usable") );
    accessAction= menu_resols->addAction( tr("Access") );
    QMenu* actionsMenu= menuBar()->addMenu( tr("Actions⇲") );;
    resetAction= actionsMenu->addAction( tr("Reset") );
    flushAction= actionsMenu->addAction( tr("Flush") );
    hotmapAction= actionsMenu->addAction( tr("Draw Hotmap") );
    exportAction= menuBar()->addAction( tr("Export"));

    exportAction->setEnabled(false);

    menuBar()->show();
}

void MainWindow::initEvents()
{
    connect(askAction, &QAction::triggered, this, &MainWindow::askSizes);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::popAbout);
    connect(initAction, &QAction::triggered, this, &MainWindow::launchInit);
    connect(evalAction, &QAction::triggered, this, &MainWindow::launchEval);
    connect(usableAction, &QAction::triggered, this, &MainWindow::launchLocalUsable);
    connect(accessAction, &QAction::triggered, this, &MainWindow::launchLocalAccess);
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetField);
    connect(flushAction, &QAction::triggered, this, &MainWindow::emptyField);
    connect(hotmapAction, &QAction::triggered, this, &MainWindow::hotmapDraw);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportPareto);

    // Gestion du bouton export grisé ou non
    connect(usableAction, &QAction::triggered, exportAction, &QAction::setDisabled);
    connect(accessAction, &QAction::triggered, exportAction, &QAction::setDisabled);
    connect(initAction, &QAction::triggered, exportAction, &QAction::setEnabled);
    connect(resetAction, &QAction::triggered, exportAction, &QAction::setEnabled);
    connect(flushAction, &QAction::triggered, exportAction, &QAction::setEnabled);
}

//@}
/// #########################
///         Others
/// #########################
//@{

void MainWindow::updateWorkField()
{

    if ( fieldWidget->has_modified_ES() /* || sizeModified*/ ) {
        resolution->changeWorkField(fieldWidget->get_field(), true);
        fieldWidget->set_unmodified_ES();
    }
    else {
        resolution->changeWorkField(fieldWidget->get_field(), false);
    }

}

std::string MainWindow::get_resolution_name() const
{
    std::ostringstream oss;
    oss<< initialField.get_width()<<"_"<< initialField.get_height()<< "_"<< resolution->get_nb_not_dominated()<<"sol";
    return oss.str();
}

#define PARETO_FOLDER std::string("../ParetoResults/")

std::string MainWindow::drawPareto(const std::string &dataName)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if (Gnuplot::set_GNUPlotPath("D:/Apps/gnuplot/bin")) {
        std::cerr << "gnuplotPath a été défini"<< endl;
    } else {
        std::cerr << "gnuplotPath n'a pas pu être défini"<< endl;
    }
#endif

    try {
        std::string paretoInputName (PARETO_FOLDER + dataName +".pareto.txt");
        std::string evalsInputName  (PARETO_FOLDER + dataName +".evaluations.txt");
        std::string outputName      (PARETO_FOLDER + "resolutionPareto" + dataName +".jpg");

//        gp.cmd("set clabel {'%8.3g'}");
//        gp.cmd("show clabel");

        Gnuplot gp;
        gp.set_terminal_std("jpeg");

        std::ostringstream title;
        title << "Front Pareto, pour une surface de taille " <<  initialField.get_width()<<" par "<< initialField.get_height();
        std::ostringstream subtitle;
        subtitle << resolution->get_nb_not_dominated() << " solution(s) non dominée(s)";
        gp.set_title(title.str() + "\\n" + subtitle.str());

        gp.set_xlabel("Nombre d'exploitables");
        gp.set_ylabel("Accessibilité moyenne");

            std::clog << "GNUPlot : "<< "set output '"+ outputName +"'"<< endl;
        gp.cmd("set output '"+ outputName +"'");
            std::clog << "GNUPlot : "<< "plot '"<< paretoInputName <<"' lc rgb 'red', '"<< evalsInputName <<"' lc rgb 'black'"<< endl;
        gp.cmd("plot '" + paretoInputName + "' lc rgb 'red', '" + evalsInputName + "' lc rgb 'black'");

        return outputName;
    }
    catch (GnuplotException& ge)
    {
        cout << "ERROR : " << ge.what() << endl;
        return "";
    }
}


//@}
/// ##############################################
///     Actions des boutons et de l'interface
/// ##############################################
//@{

#include <QDialog>
#include <QSpinBox>
const QPoint* MainWindow::askSizes()
{
    if(initWindow != nullptr){
        delete initWindow;
    }
    initWindow = new QDialog(this);
    initWindow->resize(190, 120);
    initWindow->setMaximumSize(190, 120);

    QVBoxLayout* VLay= new QVBoxLayout(initWindow);
    QLabel* width_label= new QLabel("Largeur ?");
    QSpinBox* width_spin= new QSpinBox(initWindow);
        width_spin->setValue(14); /// TODO utiliser une variabe
    QLabel* height_label= new QLabel("Hauteur ?");
    QSpinBox* height_spin= new QSpinBox(initWindow);
        height_spin->setValue(14); /// TODO utiliser une variabe

    QPushButton* valid= new QPushButton("OK", initWindow);

    VLay->addWidget(width_label);
    VLay->addWidget(width_spin);
    VLay->addWidget(height_label);
    VLay->addWidget(height_spin);
    VLay->addWidget(valid);

    width_label->setStyleSheet("QLabel {color : darkred;  }");
    height_label->setStyleSheet("QLabel {color : darkred;  }");
    connect(valid, &QPushButton::clicked, initWindow, &QDialog::close );

//    QDialog* dialog= new QInputDialog(this);
//    dialog->setLabelText("mon texte");
//    dialog->getInt(this, "Tailles et paramètres", "Largeur", 15);
//    dialog->getInt(this, "Tailles et paramètres", "Hauteur", 15);
    initWindow->exec();

    return new QPoint(width_spin->value(), height_spin->value());
}

void MainWindow::popAbout()
{
    if(aboutWindow == nullptr){
        aboutWindow = new QWidget(/*this*/);
        QVBoxLayout* VLay= new QVBoxLayout(aboutWindow);
        QLabel* signature= new QLabel("Florian DAVID");
        QLabel* about= new QLabel("Projet première année de master");
        QLabel* contact= new QLabel("Contact : <a href='mailto:f.david5@laposte.net?subject=Application Urbanisme'>f.david5@laposte.net</a>");
        QLabel* depot= new QLabel("Depot: <a href='https://github.com/flodavid/Urbanisme'>github.com/flodavid/Urbanisme</a>");
        QPushButton* valid= new QPushButton("OK", aboutWindow);
        VLay->addWidget(signature);
        VLay->addWidget(about);
        VLay->addWidget(contact);
        VLay->addWidget(depot);
        VLay->addWidget(valid);

        contact->setOpenExternalLinks(true);
        depot->setOpenExternalLinks(true);

        signature->setStyleSheet("QLabel {color : darkblue;  }");
        about->setStyleSheet("QLabel {color : darkblue;  }");
        contact->setStyleSheet("QLabel {color : darkblue;  }");
        depot->setStyleSheet("QLabel {color : darkblue;  }");
        connect(valid, SIGNAL(clicked()), aboutWindow, SLOT(close()) );
    }
    aboutWindow->show();
}

void MainWindow::launchInit()
{
    updateWorkField();

    FieldEvaluation* result_field= resolution->initResolution();

    fieldWidget->update_field(result_field);
    fieldWidget->redraw();
    fieldWidget->show();
}

void MainWindow::launchLocalUsable()
{
    updateWorkField();

    QInputDialog* widget_ask_nbMax= new QInputDialog(this);
    unsigned maxRoadsToAdd= (unsigned)widget_ask_nbMax->getInt(this, "Maximisation du nombre d'exploitables", "Nombre maximum de routes à ajouter ?\n0 pour aucune limite", 0);
    delete widget_ask_nbMax;

    FieldEvaluation* result_field= resolution->localSearchUsableObjective(maxRoadsToAdd);

    fieldWidget->update_field(result_field);

    fieldWidget->redraw();
    fieldWidget->show();
}

void MainWindow::launchLocalAccess()
{
    updateWorkField();

    QInputDialog* widget_ask_nbMax= new QInputDialog(this);
    unsigned maxPathsToAdd= (unsigned)widget_ask_nbMax->getInt(this, "Maximisation accessibilité", "Nombre chemins à ajouter ?", 1);
    delete widget_ask_nbMax;

    FieldEvaluation* result_field= resolution->localSearchAccessObjective(maxPathsToAdd);

    fieldWidget->update_field(result_field);

    fieldWidget->redraw();
    fieldWidget->show();
}

void MainWindow::launchEval()
{
    resolution->evaluateBothObjectives();
    if (resolution->get_nb_not_dominated() > (size_t)0) {
        exportAction->setEnabled(true);
    }
}

void MainWindow::resetField()
{
    std::cout << initialField<< std::endl;
    fieldWidget->set_field(new Field(initialField));
    updateWorkField();

    std::cout << "Solution trouvée supprimée"<< std::endl;

    fieldWidget->redraw();
}

void MainWindow::emptyField()
{
//    if (&initialField != fieldWidget->get_field()) {
//        delete fieldWidget->get_field();

//        std::cout << "Solution trouvée supprimée"<< std::endl;
//    }
    delete resolution;

    // Initialisation d'une nouvelle surface vide
    initialField= Field(initialField.get_width(), initialField.get_height());
    initialField.setUsables(parameters.get_serve_distance());

    fieldWidget->set_field(&initialField);
    resolution= new Resolution(initialField, parameters);

    fieldWidget->redraw();
}

void MainWindow::hotmapDraw()
{
    fieldWidget->drawHotmapField();
}

void MainWindow::exportPareto()
{
//    QFileDialog* file_browser= new QFileDialog(this);

//    file_browser->setAcceptMode(QFileDialog::AcceptSave);
//    file_browser->setNameFilter(tr("Save") +" (*.pareto.txt)");

//    if(file_browser->exec() == QDialog::Accepted){
//        std::string filename = file_browser->selectedFiles()[0].toStdString();
//        if (filename == "") {
        std::string filename= get_resolution_name() + ".pareto.txt";
//        }

        // Sauvegarde de la foret dans FireWidget qui effectue la procédure de Foret
        resolution->trySaveParetoToTxt(filename);

        exportAction->setEnabled(false);

        std::string picturePath= drawPareto(get_resolution_name());

        if (picturePath != "") {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
            std::replace(picturePath.begin(), picturePath.end(), '/', '\\');
            std::clog << "___resultat commande terminal : "<< ( system(picturePath.c_str()) )<< endl;
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
            std::string cmd= "xdg-open " + picturePath;
            std::clog << "resultat commande bash : "<< ( system(cmd.c_str()) )<< endl;
#endif
        } else std::cerr << "La création de l'image du front Pareto a échoué"<< endl;
//    }
}
//@}
