#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QMenuBar>

#include "fieldview.hpp"

#include "resolution.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Field initialField;
    Parameters parameters;

    FieldWidget* fieldWidget;

//  MENU BAR
    QWidget* aboutWidget;
    QAction* aboutAction;

    QAction* initAction;
    QAction* evalAction;
    QAction* resolAction;
    QAction* resetAction;

public:
    /**
     * Constructeur par défaut
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * Constructeur avec des valeurs initiales
     * @param parent
     */
    explicit MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent = 0);

    /**
     * Constructeur avec des valeurs initiales
     * @param parent
     */
    explicit MainWindow(unsigned nbCols, unsigned nbRows, const Parameters& params, QWidget *parent = 0);

    void askSizes();

    void askParams();

    /**
     * @brief initComponents
     */
    void initComponents();

    /**
     * @brief initEvents
     */
    void initEvents();

    /* Getters */
    FieldWidget* get_fieldWidget() { return fieldWidget; }

    Field& get_initialField() { return initialField; }

    Parameters& get_parameters() { return parameters; }

signals:

public slots:
    /**
     * Affiche la fenêtre de présentation
     */
    void popAbout();

    /**
     * @brief launchInit
     */
    void launchInit();

    /**
     * @brief launchResol
     */
    void launchResol();

    /**
     * @brief launchEval
     */
    void launchEval();

    /**
     * @brief resetField
     */
    void resetField();
};

#endif // MAINWINDOW_H
