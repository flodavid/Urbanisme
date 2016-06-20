#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QProgressBar>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/qevent.h>

#include <vector>
#include <fstream>

// Macros
#include "../debug.h"
#include "../stdafx.h"

// Classes
#include "fieldevaluation.h"
#include "loadwindow.h"

enum Colors{Black, Gray, Red, White, LightBlue, DarkBlue};
//enum Actions{};

/**
 * @brief Widget d'affichage de l'automate cellulaire. Ce composant
 * permet de suivre graphiquement l'évolution de l'automate.
 * Il utilise un système de buffering et s'appuie sur la classe
 * Foret du moteur.
 */
class FieldWidget : public QWidget {
//    Q_OBJECT
private:
    Field* field;
    unsigned serveDistance;

    QImage* buffer;
    QColor* color;
    QPainter* bufferPainter;
    QRubberBand* rubber;

//    QPoint origin;

    // Points de départ et d'arrivée de la zone de selection (redondance pour origine mais normal pour l'instant)
//    QPoint depart;
//    QPoint arrivee;

    qreal tailleCell;
//    long temps;
//    bool running;

    std::list<Coordinates> selecteds;

    // FLAGS
    bool modified_ES;
    bool has_evaluation;

private:
    void initRubber(QMouseEvent* event);

public:
    /* Constructeur et desctructeur */
    /**
     * Constructeur à partir d'une surface déjà crée.
     * Initialise les différents pointeurs et fixe la taille minimale du widget
     * @param _field Surface associée au widget, c'est elle qui est affichée
     * @param _serveDistance Distance de desserte des routes
     */
    FieldWidget(Field *_field, unsigned _serveDistance);
    virtual ~FieldWidget();

    /* Getters */
    /**
     * Accesseur sur la surface affichée
     * @return Un Field, modifiable
     */
    Field* get_field()
    { return field; }

    /**
     * Accesseurs sur modified_ES
     * @return modified_ES, un booléen
     */
    bool has_modified_ES() const { return modified_ES; }

    /* Setters */
    /**
     * Change la surface utilisée
     * @param _field Nouvelle surface affichée
     */
    void set_field(Field* _field)
    { field= _field; has_evaluation= false;}
    /**
     * Les routes de la surface utilisée sont mise à jour pour l'affichage, mais on suppose que les
     * entrée et sorties sont identiques
     * @param _field Surface à afficher mise à jour
     */
    void update_field(FieldEvaluation* _field);

    /**
     * Remet la valeur du "flag" modified_ES à faux
     */
    void set_unmodified_ES();

private:
    /**
     * Fixe la couleur à utiliser pour dessiner un arbre
     * @param colorIndice Indice de la couleur de la case, parmi ceux de l'enum 'Colors'
     */
    void setColor(Colors colorIndice);
    /**
     * Fixe la couleur à utiliser pour dessiner un arbre
     * @param r Quantité de rouge, max 255
     * @param g Quantité de vert, max 255
     * @param b Quantité de bleu, max 255
     * @param a Transparence des couleurs, max 255, par défaut 255 (opaque)
     */
    void setColor(int r, int g, int b, int a = 255);

    /* Affichage */
private:
    /**
     * Imprime une cellule à une position donnée, utilise la couleur courante
     * @param colonne,ligne indices de la colonne et de la ligne de la cellules
     */
    void drawCell(int colonne, int ligne);
    /**
     * Dessine l'ensemble des arbres de la liste passée en paramètre
     * @param list_coordinates liste d'emplacement des coordonnées à dessiner
     */
    void drawList(const std::list< Coordinates>& list_coordinates);

    /**
     * Dessine toutes les cellules dans le buffer
     */
    void drawField();
public:
    /**
     * Dessine la surface avec les "points chaud" de mauvaise accessibilité
     */
    void drawHotmapField();
    /**
     * Vide le buffer et rafraichit l'affichage
     */
    void redraw();

    /* Initialisations */
private:
    LoadWindow* createProgressWindow() const;

    /* Gestion Sauvegardes */
    /**
     * Sauvegarde la surface sous forme d'image
     * @param filename chemin du nouveau fichier de sauvegarde
     * @return vrai si la sauvegarde réussi
     */
    bool trySaveImage(QString filename) const;

    /* Events */
private:
    /**
     * Tente d'ajouter une route à une position donnée.
     * @param pos Position de la nouvelle route
     * @return faux si il y a déjà une route présente à cette position ;
     * Si il y a une E/S, aucune route n'est ajoutée, mais on retourne vrai, car ce n'est pas une route
     */
    bool tryAddRoadOnParcel(const Coordinates& pos);

    /**
     * Tente de placer une E/S selon une position donnée
     * @param pos Emplacement de l'entrée/sortie
     */
    void clicInOut(const Coordinates& pos);
    /**
     * Place une route selon une position donnée, si il n'y a pas d'entrée sortie présente.
     * Si une route est déjà présente, elle est enlevée
     * @param pos Emplacement de la route
     */
    void clicRoad(const Coordinates& pos);
    /**
     * Tente de placer une route selon une position donnée
     * @param pos Emplacement de la route
     */
    void moveRoad(const Coordinates& pos);
    /**
     * Ajoute la parcelle désignée dans la liste des parcelles sélectionnée ;
     * Si il y a déjà deux parcelles sélectionnées, elle remplace la plus ancienne.
     * Si il y deux parcelles après l'ajout de celle-ci, leur distance par les routes est calculée
     * TODO afficher le résultat de cette distance : signal vers MainWindow ?
     * @param pos Emplacement de la route
     */
    void selectParcel(const Coordinates& pos);

public slots:
    /**
     * Réinitialise le rubber
     */
    void razRubber() 	{ delete rubber; rubber= NULL; }

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent* event);

public slots:
    /**
     * Execute l'action choisie sur une zone, suite à releaseMouseEvent.
     * @param x Identifiant de l'action reçue
     */
//    void actionReceived(int x);

signals:
    /**
     * Récupere l'action sélectionnée dans les menus
     */
//    void releaseSignal(); // Vers ...

};
