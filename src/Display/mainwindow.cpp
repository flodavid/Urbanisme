#include "mainwindow.hpp"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAction>
#include <QtGui/QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), initialField(0,0), parameters(0,0), fieldWidget(nullptr)
{
    aboutWidget= nullptr;
//    askSizes();
//    askParams();

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(serveDistance, roadsWidth)
{
    fieldWidget= new FieldWidget(&initialField);

    aboutWidget= nullptr;

    initComponents();
    initEvents();
}

MainWindow::MainWindow(unsigned nbCols, unsigned nbRows, const Parameters &params, QWidget *parent)
    : QMainWindow(parent), initialField(nbCols, nbRows), parameters(params)
{
    fieldWidget= new FieldWidget(&initialField);
    initialField.defineUsables(params.get_serve_distance());

    aboutWidget= nullptr;

    initComponents();
    initEvents();
}

void MainWindow::initComponents()
{
    setCentralWidget(fieldWidget);
    fieldWidget->redraw();
    fieldWidget->show();

    // Menu bar
    aboutAction= menuBar()->addAction( tr("About") );

    initAction= menuBar()->addAction( tr("Initialisation") );
}

void MainWindow::initEvents()
{
    connect(aboutAction, &QAction::triggered, this, &MainWindow::popAbout);

    connect(initAction, &QAction::triggered, this, &MainWindow::launchInit);
}

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
    Resolution myResolution(initialField, parameters);

    myResolution.initResolution(initialField);
//    FieldWidget* resultFieldWidget= new FieldWidget();

//    this->hide();

    fieldWidget->redraw();
    fieldWidget->show();

//    resultFieldWidget->show();

//    myResolution.launchResolution();
}
