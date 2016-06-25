﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMenuBar>

#include "Engine/field.h"
#include "Engine/parameters.h"
#include "fieldevaluation.h"
#include "resolution.h"

#include "fieldview.hpp"

/**
 * @brief Fenêre principale de l'application. Chargée d'afficher la surface, les menus, les
 * résultats et gérer le lancement des opérations
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Données
    Field initialField;
    Parameters parameters;
    Resolution* resolution;

    // Widgets
    FieldWidget* fieldWidget;

    //  MENU BAR
    QAction* askAction;
    QAction* aboutAction;
    QAction* initAction;
    QAction* evalAction;
    QAction* usableAction;
    QAction* accessAction;
    QAction* hotmapAction;
    QAction* resetAction;
    QAction* flushAction;
    QAction* exportAction;
    QAction* change_fieldAction;

    // Fenêtres
    QWidget* aboutWindow;
    QDialog* initWindow;

public:
    /**
     * Constructeur par défaut
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * Constructeur à partir d'une surface déjà crée
     * @param field Surface à utiliser
     * @param params Paramètres de résolution
     * @param parent Widget parent
     */
    explicit MainWindow(const Field& field, const Parameters& params, QWidget *parent = 0);

//    /**
//     * Constructeur avec des valeurs initiales
//     * @param nbCols Largeur de la surface
//     * @param nbRows Hauteur de la surface
//     * @param serveDistance Distance de desserte de la résolution
//     * @param roadsWidth Largeur des routes de la résolution
//     * @param parent Widget parent
//     * @oldcode
//     */
//    explicit MainWindow(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, QWidget *parent = 0);

//    /**
//     * Constructeur avec des valeurs initiales
//     * @param nbCols Largeur de la surface
//     * @param nbRows Hauteur de la surface
//     * @param params Paramètres de résolution
//     * @param parent Widget parent
//     * @oldcode
//     */
//    explicit MainWindow(unsigned nbCols, unsigned nbRows, const Parameters& params, QWidget *parent = 0);

    /**
     * Destructeur de la classe MainWindow
     */
    virtual ~MainWindow();

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
    const Field& get_initialField() { return initialField; }

    /* Others */
//private:
    void updateWorkField();

private:
    std::string get_resolution_name() const;

    /* Actions des boutons et de l'interface */

    /**
     * Demande les tailles de la surface à traiter à l'utilisateur
     */
    void askNewParams();

private slots:
//    /**
//     * Demande les paramètres du problème avec lesquels effectuer la résolution
//     */
//    void askParams();
    /**
     * Affiche la fenêtre de présentation
     */
    void popAbout();
    /**
     * Lance le placement des premières routes à partir des deux E/S
     */
    void launchInit();
    /**
     * Lance une maximisation du nombre de parcelles exploitables
     * @see voir si on ajoute un nombre définit de routes : maxToAdd
     */
    void launchLocalUsable();
    /**
     * Lance une maximisation de l'accessibilité
     * @see, on pourrait fixer un seuil de gain minimum et ajouter toutes les solutions ou
     * ajouter un nombre définit de chemins maximums (ou de routes ?) à ajouter au lieu d'un seul
     * Ou chercher le maximum, puis choisir tous les chemins proches, selon un delta
     * @see prendre en compte la perte des parcelles engendré par l'ajout de routes ?
     */
    void launchLocalAccess();
    /**
     * Lance l'évaluation des deux objectifs
     * TODO afficher les résultats dans la fenêtre plutôt que dans le terminal
     */
    void launchEval();
    /**
     * Supprime toutes les routes ajoutées par les algorithmes de résolution,
     * seules celles placées par l'utilisateur restent
     */
    void resetField();
    /**
     * Supprime toutes les routes de la surface
     */
    void emptyField();
    /**
     * Dessine la surface avec les points chauds de mauvaise accessibilité
     */
    void hotmapDraw();
    /**
     * Demande à l'utilisateur l'emplacement et le nom du fichier d'export du front Pareto
     */
    void exportPareto();
    /**
     * Demande à l'utilisateur l'indice de la surface, parmis celles sauvegardées, qu'il veut
     * afficher et utilier.
     */
    void askChangeField();
    /**
     *  Crée une nouvelle surface en demandant sa taille
     */
    void newWorkField();
};

#endif // MAINWINDOW_H
