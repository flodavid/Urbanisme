#pragma once

#include "../stdafx.h"

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

// Classes
#include "../Engine/Field.h"
#include "loadwindow.h"

enum Colors{Black, Gray, Red, White, LightBlue, DarkBlue};
//enum Actions{Black, Gray, Red, White, LightBlue};

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

private:
    void initRubber(QMouseEvent* event);

public:
    /* Constructeur et desctructeur */
    /**
     * Constructeur à partir d'une surface déjà crée.
     * Initialise les différents pointeurs et fixe la taille minimale du widget
     * @param _field Surface associée au widget, c'est elle qui est affichée
     */
    FieldWidget(Field *_field, unsigned _serveDistance);
    virtual ~FieldWidget();

    /* Getters */
    Field* get_field()
    { return field; }

    /* Setters */
    void set_field(Field* _field)
    { field= _field; }

private:
    /**
     * Fonction permettant de fixer la couleur à utiliser pour dessiner un arbre
     * @param colorIndice Indice de la couleur de la case, parmi ceux de l'enum 'Colors'
     */
    void setColor(Colors colorIndice);

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

public:
    /**
     * Dessine toutes les cellules dans le buffer
     */
    void drawField();
    /**
     * Redessine les cellules qui ont changés d'état seulement
     */
    void drawChanged();
    /**
     * Dessine les cellules sélectionnées
     */
    void drawSelecteds();
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
    void clicInOut(const Coordinates& pos);

    void clicRoad(const Coordinates& pos);

    void moveRoad(const Coordinates& pos);

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
