#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QMenuBar>

#include "fieldview.hpp"

#include "resolution.h"

/**
 * @brief Fenêre principale de l'application. Chargée d'afficher la surface, les menus, les
 * résultats et gérer le lancement des opérations
 */
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
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param serveDistance Distance de desserte de la résolution
     * @param roadsWidth Largeur des routes de la résolution
     * @param parent Widget parent
     * @param parent
     */
    explicit MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent = 0);

    /**
     * Constructeur avec des valeurs initiales
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param params Paramètres de résolution
     * @param parent Widget parent
     */
    explicit MainWindow(unsigned nbCols, unsigned nbRows, const Parameters& params, QWidget *parent = 0);

    /**
     * Demande les tailles de la surface à traiter à l'utilisateur
     */
    void askSizes();

    /**
     * Demande les paramètres du problème avec lesquels effectuer la résolution
     */
    void askParams();

    /**
     * Créer et instancie les composants graphiques
     */
    void initComponents();

    /**
     * Créer les connections des signaux et slots de la fenêtre
     */
    void initEvents();

    /* Getters */
    /**
     * Accesseur sur la surface initiale
     * @return initialField, une Surface, modifiable
     */
    Field& get_initialField() { return initialField; }

signals:

public slots:
    /**
     * Affiche la fenêtre de présentation
     */
    void popAbout();

    /**
     * Lance le placement des premières routes à partir des deux E/S
     */
    void launchInit();

    /**
     * Lance la résolution du problème :
     * Maximisation du nombre de routes exploitables, puis maximisation de l'accessibilité
     */
    void launchResol();

    /**
     * Lance l'évaluation des deux objectifs
     * TODO afficher les résultats dans la fenêtre plutôt que dans le terminal
     */
    void launchEval();

    /**
     * Supprime toutes les routes autres que des E/S de la surface
     */
    void resetField();
};

#endif // MAINWINDOW_H
