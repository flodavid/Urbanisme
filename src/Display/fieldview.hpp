#ifndef FIREWIDGET_H
#define FIREWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QRubberBand>
#include <QtWidgets/QProgressBar>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/qevent.h>	// IMPROVEIT pas d'equivalent sans .h ?
#include <ctime>

#include <vector>
#include <fstream>

// Macros
#include "../debug.h"

// Classes
#include "../Engine/Field.h"
#include "loadwindow.h"

enum Colors{Black, Gray, Red, White, LightBlue};


/**
 * Widget d'affichage de l'automate cellulaire. Ce composant
 * permet de suivre graphiquement l'évolution de l'automate.
 * Il utilise un système de buffering et s'appuie sur la classe
 * Foret du moteur.
 * @author Ugo et Florian
 */
class FieldWidget : public QWidget {
//    Q_OBJECT
private:
    const Field* field;
    QImage* buffer;
    QColor* color;
    QPainter* bufferPainter;

    QPoint origin;
    QRubberBand* rubber;
    // Points de départ et d'arrivée de la zone de selection (redondance pour origine mais normal pour l'instant)
    QPoint depart;
    QPoint arrivee;

    qreal tailleCell;
    long temps;
    bool running;

private:
    void initRubber(QMouseEvent* event);

public:
    /* Constructeur et desctructeur */
    /**
     * Initialise les différents pointeurs et fixe la taille minimale du widget
     * @author Ugo et Florian
     *
     * @param int _largeur : nombre de colonnes de la matrice
     * @param int _hauteur : nombre de lignes de la matrice
     * @param float _proba : probabilité qu'une cellule deviennent un arbre
     * @param float _coef : coefficient de combustion de l'incendie
     */
    FieldWidget(const Field *_field);
    virtual ~FieldWidget();

private:
    LoadWindow* createProgressWindow() const;
    /**
     * Fonction permettant de fixer la couleur à utiliser pour dessiner un arbre
     * @param Colors indice de la couleur de la case, parmi ceux de l'enum Colors
     * @author Florian et Ugo
     */
    void setColor(Colors colorIndice);

    // Setters
public slots:
public:
    void razRubber() 	{ rubber= NULL; }

    /* Getters */
    int getTailleCell() const { return tailleCell; }

    /* Gestion Sauvegardes */
    /**
     * Sauvegarde la forêt sous forme d'image
     * @author Florian
     * @param filePath chemin du nouveau fichier de sauvegarde
     * @return vrai si la sauvegarde réussi
     */
    bool trySaveImage(QString filename) const;

    /* Affichage */
    /**
     * Imprime une cellule à une position donnée, utilise la couleur courante
     * @author Florian
     * @param int col,row indices de la colonne et de la ligne de la cellules
     */
    void drawCell(int colonne, int ligne);
    /**
     * Dessine l'ensemble des arbres de la liste passée en paramètre
     * @param arbres liste des arbres à dessiner
     * @author Florian et Ugo (commentaires :p )
     */
    void drawList(std::list< Coordinates* >* list_coordinates);
    /**
     * Dessine les arbres et cellules vides dans le buffer
     * @author Ugo et Florian
     */
    void drawField();
    /**
     * Redessine les arbres qui ont changés d'état, sur l'ancienne matrice
     * On réutilise les cellules non susceptibles d'avoir été modifiées
     * @author Florian and Ugo
     */
    // IMPROVEIT faire une fonction qui prend une couleur et une liste d'arbres, qui "imprime" les arbres avec cette couleur ?
    void drawChanged();
    /**
     * Vide le buffer et rafraichit l'affichage
     * @author Florian et Ugo
     */
    void redraw();

protected:
    /* Events */
    // TODO comments
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent * event);
    void paintEvent(QPaintEvent* event);

public slots:
    /**
     * Execute l'action choisie sur une zone, suite à releaseMouseEvent.
     * 0 correspond à une coupure, 1 à un retardateur
     * @author Ugo
     */
    void actionReceived(int x);

//signals:
//    /**
//     * Récupere l'action sélectionnée dans les menus
//     *
//     * @author Ugo
//     * TODO Ugo comment
//     */
//    void releaseSignal(); // Vers firescreen

};

#endif // FIREWIDGET_H

