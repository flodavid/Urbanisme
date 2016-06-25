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
#include <QDialog>
#include <QSpinBox>

using std::cout; using std::cerr; using std::endl;

/// #########################
///      Constructeurs
/// #########################
//@{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), initialField(0,0), parameters(0,0)
{
    initWindow= nullptr;
    askNewParams();

    fieldWidget= new FieldWidget(&initialField, parameters.get_serve_distance());

    resolution= new Resolution(initialField, parameters);

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

//MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent)
//    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(serveDistance, roadsWidth)
//{
//    fieldWidget= new FieldWidget(&initialField, serveDistance);
//    initialField.setUsables(parameters.get_serve_distance());

//    resolution= new Resolution(initialField, parameters);

//    initComponents();
//    initEvents();
//}

//MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, const Parameters &params, QWidget *parent)
//    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(params)
//{
//    fieldWidget= new FieldWidget(&initialField, params.get_serve_distance());
//    initialField.setUsables(parameters.get_serve_distance());

//    resolution= new Resolution(*(fieldWidget->get_field()), parameters);

//    initComponents();
//    initEvents();
//}

MainWindow::~MainWindow()
{
    if (aboutWindow != nullptr) {
        delete aboutWindow;
    }
    if(initWindow != nullptr){
        delete initWindow;
    }

    delete aboutAction;
    delete initAction;
    delete evalAction;
    delete usableAction;
    delete accessAction;
    delete resetAction;
    delete flushAction;
    delete exportAction;
    delete change_fieldAction;

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
    change_fieldAction= actionsMenu->addAction( tr("Change field"));
    exportAction= menuBar()->addAction( tr("Export"));

    exportAction->setEnabled(false);

    menuBar()->show();
}

void MainWindow::initEvents()
{
    connect(askAction,      &QAction::triggered, this, &MainWindow::newWorkField);
    connect(aboutAction,    &QAction::triggered, this, &MainWindow::popAbout);
    connect(initAction,     &QAction::triggered, this, &MainWindow::launchInit);
    connect(evalAction,     &QAction::triggered, this, &MainWindow::launchEval);
    connect(usableAction,   &QAction::triggered, this, &MainWindow::launchLocalUsable);
    connect(accessAction,   &QAction::triggered, this, &MainWindow::launchLocalAccess);
    connect(resetAction,    &QAction::triggered, this, &MainWindow::resetField);
    connect(flushAction,    &QAction::triggered, this, &MainWindow::emptyField);
    connect(hotmapAction,   &QAction::triggered, this, &MainWindow::hotmapDraw);
    connect(change_fieldAction, &QAction::triggered, this, &MainWindow::askChangeField);
    connect(exportAction,   &QAction::triggered, this, &MainWindow::exportPareto);

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

//@}
/// ##############################################
///     Actions des boutons et de l'interface
/// ##############################################
//@{

void MainWindow::askNewParams()
{
    if(initWindow != nullptr){
        delete initWindow;
    }
    initWindow = new QDialog(this);
    initWindow->resize(190, 120);
    initWindow->setMaximumSize(190, 120);

    // Layout de la fenêtre
    QVBoxLayout* VLay= new QVBoxLayout(initWindow);

    // Eléments pour entrer la distance de desserte
    QLabel* dist_label= new QLabel( tr("Distance de desserte ?") );
    QSpinBox* dist_spin= new QSpinBox(initWindow);
        dist_spin->setValue(2);
    VLay->addWidget(dist_label);
    VLay->addWidget(dist_spin);

//    // Eléments pour entrer la largeur des routes
//    QLabel* road-width_label= new QLabel( tr("Distance de desserte ?") );
//    QSpinBox* road-width_spin= new QSpinBox(initWindow);
//        road-width_spin->setValue(1);
//    VLay->addWidget(road-width_label);
//    VLay->addWidget(road-width_spin);

    // Eléments pour entrer la largeur
    QLabel* width_label= new QLabel(tr("Largeur ?"));
    QSpinBox* width_spin= new QSpinBox(initWindow);
        width_spin->setValue(14);
    VLay->addWidget(width_label);
    VLay->addWidget(width_spin);

    // Eléments pour entrer la hauteur
    QLabel* height_label= new QLabel(tr("Hauteur ?"));
    QSpinBox* height_spin= new QSpinBox(initWindow);
        height_spin->setValue(14);
    VLay->addWidget(height_label);
    VLay->addWidget(height_spin);

    QPushButton* valid= new QPushButton("OK", initWindow);
    VLay->addWidget(valid);

    width_label->setStyleSheet("QLabel {color : darkred;  }");
    height_label->setStyleSheet("QLabel {color : darkred;  }");
    dist_label->setStyleSheet("QLabel {color : darkred;  }");

    connect(valid, &QPushButton::clicked, initWindow, &QDialog::close );

    initWindow->exec();

// Utilisation des valeurs entrées
    parameters.set_serve_distance(2);
    parameters.set_road_width(1); /// @see changer si la largeur des routes est prise en compte

    Field tmp_field= Field(width_spin->value(), height_spin->value());
        tmp_field.setUsables(parameters.get_serve_distance());
    initialField= tmp_field;
}

void MainWindow::popAbout()
{
    if(aboutWindow == nullptr){
        aboutWindow = new QWidget();
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
}

void MainWindow::launchEval()
{
    resolution->evaluateBothObjectives();
    if (resolution->get_nb_not_dominated() > (size_t)0) {
        exportAction->setEnabled(true);
    }
    fieldWidget->update();
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
    initialField.resizeWithDimensions();
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
        if (!resolution->trySaveParetoToTxt(filename)) {
            cerr<< "ERROR : Echec lors de la sauvegarde du front pareto sous forme de texte"<< endl;
        }

        exportAction->setEnabled(false);

        std::string picturePath= resolution->drawParetoJpeg(get_resolution_name());

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

void MainWindow::askChangeField()
{
    QInputDialog* dialog= new QInputDialog(this);
//    dialog->setLabelText( tr("Changement de surface courante") );
    std::ostringstream question_indice;
    question_indice << "Indice (1-" << resolution->get_nb_not_dominated() << ") :";

    int ind= dialog->getInt(this, "Choix de la surface à restaurer", QString::fromStdString(question_indice.str()), 0);
    cout << "INDICE : "<< ind<< endl;
    if (ind <= 0 || (unsigned)ind > resolution->get_nb_not_dominated()) {
        cerr << "Il n'y a pas de surface correspondant à l'indice donné."<< endl
             << "Pour répéter changer de surface, répétez l'opération avec un nouvel indice"<< endl;
    } else {
        FieldEvaluation* selected_field= resolution->trySelectSavedField(ind-1);
        if (selected_field != nullptr) {
            fieldWidget->update_field(selected_field);
            fieldWidget->redraw();
        } else {
            cerr<< "ERROR : Echec lors du changement de surface"<< endl;
        }
    }
}

void MainWindow::newWorkField()
{
    askNewParams();

    fieldWidget->set_field(&initialField);
    fieldWidget->resize(initialField.get_width() * INITIAL_SCALE , initialField.get_height() * INITIAL_SCALE);
    fieldWidget->redraw();
//    resolution->changeWorkField(new_field, true);
}

//@}
