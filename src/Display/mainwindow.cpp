#include "mainwindow.hpp"

#include <QtGui/QDesktopServices>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtGui/QMouseEvent>

/// #########################
///      Constructeurs
/// #########################
//@{
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), initialField(0,0), parameters(0,0), fieldWidget(nullptr)
{
    //    askSizes();
    //    askParams();

    resolution = nullptr;

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(serveDistance, roadsWidth)
{
    fieldWidget= new FieldWidget(&initialField, serveDistance);

    resolution = nullptr;

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, const Parameters &params, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(params)
{
    fieldWidget= new FieldWidget(&initialField, params.get_serve_distance());
    initialField.setUsables(params.get_serve_distance());

    resolution = nullptr;

    initComponents();
    initEvents();
}

#define INITIAL_SCALE 32
void MainWindow::initComponents()
{
    aboutWidget= nullptr;

    resize(initialField.get_width() * INITIAL_SCALE , initialField.get_height() * INITIAL_SCALE +20);

    setCentralWidget(fieldWidget);
    fieldWidget->redraw();
    fieldWidget->show();

    // Menu bar
    aboutAction= menuBar()->addAction( tr("About") );
    initAction= menuBar()->addAction( tr("Initialise") );
    evalAction= menuBar()->addAction( tr("Evaluate") );
    QMenu* menu_resols= menuBar()->addMenu( tr("Resolutions") );
    usableAction= menu_resols->addAction( tr("Usable") );
    accessAction= menu_resols->addAction( tr("Access") );
    resetAction= menuBar()->addAction( tr("Reset") );
    flushAction= menuBar()->addAction( tr("Flush") );
    exportAction= menuBar()->addAction( tr("Export"));

    exportAction->setEnabled(false);
}

void MainWindow::initEvents()
{
    connect(aboutAction, &QAction::triggered, this, &MainWindow::popAbout);
    connect(initAction, &QAction::triggered, this, &MainWindow::launchInit);
    connect(evalAction, &QAction::triggered, this, &MainWindow::launchEval);
    connect(usableAction, &QAction::triggered, this, &MainWindow::launchLocalUsable);
    connect(accessAction, &QAction::triggered, this, &MainWindow::launchLocalAccess);
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetField);
    connect(flushAction, &QAction::triggered, this, &MainWindow::emptyField);
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
///         Events
/// #########################
//@{

//@}
/// #########################
///         Slots
/// #########################
//@{

void MainWindow::popAbout()
{
    if(aboutWidget == nullptr){
        aboutWidget = new QWidget();
        QVBoxLayout* VLay= new QVBoxLayout(aboutWidget);
        QLabel* signature= new QLabel("Florian DAVID");
        QLabel* about= new QLabel("Projet première année de master");
        QLabel* contact= new QLabel("Contact : <a href='mailto:f.david5@laposte.net?subject=Application Urbanisme'>f.david5@laposte.net</a>");
        QLabel* depot= new QLabel("Depot: <a href='https://github.com/flodavid/Urbanisme'>github.com/flodavid/Urbanisme</a>");
        QPushButton* valid= new QPushButton("OK", aboutWidget);
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
        connect(valid, SIGNAL(clicked()), aboutWidget, SLOT(close()) );
    }
    aboutWidget->show();
}

void MainWindow::launchInit()
{
    Resolution myResolution(*(fieldWidget->get_field()), parameters);

    Field& result_field= myResolution.initResolution();

    fieldWidget->set_field(&result_field);

    fieldWidget->redraw();
    fieldWidget->show();
}

#include <QtWidgets/QInputDialog>
void MainWindow::launchLocalUsable()
{
//    if (resolution != nullptr) {
//        delete resolution;
//    }

    QInputDialog* widget_ask_nbMax= new QInputDialog(this);

    unsigned maxRoadsToAdd= (unsigned)widget_ask_nbMax->getInt(this, "Nombre maximum de routes à ajouter ?", "0 pour aucune limite", 0);

    if (resolution == nullptr) {
        resolution= new Resolution(*(fieldWidget->get_field()), parameters);
    }

    Field& result_field= resolution->localSearchUsableObjective(maxRoadsToAdd);

    fieldWidget->set_field(&result_field);

    fieldWidget->redraw();
    fieldWidget->show();
}

void MainWindow::launchLocalAccess(unsigned maxPathsToAdd)
{
//    if (resolution != nullptr) {
//        delete resolution;
//    }
    if (resolution == nullptr) {
        resolution= new Resolution(*(fieldWidget->get_field()), parameters);
    }

    Field& result_field= resolution->localSearchAccessObjective(maxPathsToAdd);

    fieldWidget->set_field(&result_field);

    fieldWidget->redraw();
    fieldWidget->show();
}

void MainWindow::launchEval()
{
    Resolution myResolution(*(fieldWidget->get_field()), parameters);

    myResolution.evaluateBothObjectives();
}

void MainWindow::resetField()
{
    if (&initialField != fieldWidget->get_field()) {
        delete fieldWidget->get_field();
        fieldWidget->set_field(&initialField);

        std::cout << "Solution trouvée supprimée"<< std::endl;
    }

    fieldWidget->redraw();
}

void MainWindow::emptyField()
{
    if (&initialField != fieldWidget->get_field()) {
        delete fieldWidget->get_field();

        std::cout << "Solution trouvée supprimée"<< std::endl;
    }

    initialField= Field(initialField.get_width(), initialField.get_height());
    initialField.setUsables(parameters.get_serve_distance());

    fieldWidget->set_field(&initialField);

    fieldWidget->redraw();
}

void MainWindow::exportPareto()
{
    QFileDialog* file_browser= new QFileDialog(this);

    file_browser->setAcceptMode(QFileDialog::AcceptSave);
    file_browser->setNameFilter(tr("Save") +" (*.pareto.txt)");

    if(file_browser->exec() == QDialog::Accepted){
        std::string filename = file_browser->selectedFiles()[0].toStdString();
        std::cout <<"taille de "<< filename<< " : "<< filename.length() << std::endl;

        // Sauvegarde de la foret dans FireWidget qui effectue la procédure de Foret
        resolution->trySaveParetoToTxt(filename);

        exportAction->setEnabled(false);
    }
}

//@}
