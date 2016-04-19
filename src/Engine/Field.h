#pragma once
#ifndef FIELD_H
#define FIELD_H

#include <list>
#include <vector>

#include "coordinates.h"

enum State{is_undefined= -1, is_road, is_usable, is_unusable};

class Field
{

private:
    unsigned nb_cols, nb_rows;
    std::list<Coordinates> ins_outs;

    std::vector<std::vector<State>> parcels;

public:
    Field(unsigned width, unsigned height/*, std::list< Coordinates >& inputs_and_ouputs = *(new std::list<Coordinates>()) */);
    ~Field();

/*=== Getters ===*/
    inline unsigned get_width() const	{ return nb_cols; }
    inline unsigned get_height() const	{ return nb_rows; }

    unsigned getNbParcels() const	{ return nb_cols * nb_rows; }

/*=== Setters ===*/
    inline void set_width(unsigned width)
        { nb_cols =  width; }
    inline void set_height(unsigned height)
        { nb_rows =  height; }

//private:
    void resizeWithDimensions();
public:
    /**
    * Ajoute une entrée/sortie à partir de coordonnées
    * @param coord Coordonnées de l'E/S
    */
    void add_in_out(const Coordinates& coord)
    { ins_outs.push_back(coord); }
    /**
    * Ajoute une entrée/sortie à partir de coordonnées
    * @param col Colonne de l'E/S
    * @param row Ligne de l'E/S
    */
    void add_in_out(unsigned col, unsigned row)
    { ins_outs.push_back(Coordinates(col, row)); }

    /**
    * Ajoute une route à partir de coordonnées
    * @param col Colonne de la route
    * @param row Ligne de la route
    */
    void add_road(unsigned col, unsigned row)
    { parcels[row][col]= is_road; }

/**=== Operators ===**/
public:
    inline State at(const Coordinates& pos) const { return parcels[pos.row][pos.col]; }
    State operator[](const Coordinates& pos) const { return parcels[pos.row][pos.col]; }

    std::vector<std::vector<State>>::const_iterator begin() const { return parcels.cbegin(); }
    std::vector<std::vector<State>>::const_iterator end() const { return parcels.cend(); }

    std::vector<std::vector<State>>::iterator begin() { return parcels.begin(); }
    std::vector<std::vector<State>>::iterator end() { return parcels.end(); }

//    friend std::vector<std::vector<State>>::const_iterator begin(const Field& f) { return f.begin(); }
//    friend std::vector<std::vector<State>>::const_iterator end(const Field& f) { return f.end(); }
    friend std::vector<State>::const_iterator& std::vector<State>::const_iterator::operator++();

    /**
     * Retourne la première coordonée de la matrice, elle peut ensuite
     * être modifée pour servir d'itérateur à l'aide de nextCoordinates()
     * @return une référence, non constante, sur la première coordonnée de la matrice
     */
    static inline Coordinates& first()
    { return *(new Coordinates(0,0)); }

public:
/* Affichage */
    void show_ins_and_outs() const;

    void show_states() const;

/**=== Fonctions sur les coordonnées ===**/
    /**
    * Effectue un test d'appartenance des coordonnées à la matrice
    * @param x abscisse de la coordonnée
    * @param y ordonnée de la coordonnée
    * @return true si la coordonnée appartient à la matrice
    */
    bool contains(int x, int y) const ;
    /**
    * Effectue un test d'appartenance des coordonnées à la matrice
    * @param coord Coordonnées de la position
    * @return true si la coordonnée appartient à la matrice
    */
    bool contains(const Coordinates& coord) const ;
    /**
     * Modifie la coordonnée en entrée pour obtenir celle qu'elle précède
     * Agit comme une itération sur l'ensemble des coordonnées de la surface
         * @param coord Coordonnée courante, va être modifiée pour devenir sa "suivante".
         * Elle est non modifiée si il n'y a pas de suivante.
     * @return vrai si il y a une coordonnées,
     * faux si on est à la fin ou en dehors de la surface
     */
    bool nextCoordinates ( Coordinates* coord ) const;

/**=== Méthodes générales	===**/
    /** Crée aléatoirement des entrées et sorties
    * @param nb nombre d'entrées et sorties à générer
    */
    void generateInsAndOuts(unsigned nb);

private:
    /**
     * Vérifie que deux parcelles sont voisines, supposant une distance de voisinage
        * @param neighbour Coordonnées de la parcelle qui peut être voisine de l'autre
        * @param coord Coordonnées de la seconde parcelle
     * @return vrai si la première parcelle est une route, voisine de la seconde
     */
    bool isRoadAndNeighbourOf(const Coordinates& neighbour, const Coordinates& coord, unsigned servingDistance) const;

public:
    /**
    * Recherche des portions de routes qui sont collées à la parcelle courante
    * @param coord Coordonnées de la parcelle
    * @return une liste de routes adjacentes à la parcelle
    */
    std::list<Coordinates> *getNeighbourRoads ( const Coordinates& coord ) const;
    /**
    * Recherche des portions de routes qui peuvent désservir la parcelle
    * @param coord Coordonnées de la parcelle desservir
    * @return une liste de routes distance inférieure ou égale à la distance de desserte
    */
    std::list<Coordinates> *getServingRoads ( const Coordinates& coord, unsigned servingDistance ) const;

private:
    /**
    * Vérifie que la parcelle a un voisin
    * @param coord Coordonnées de la parcelle desservir
    * @return une liste de routes distance inférieure ou égale à la distance de desserte
    */
    bool hasServingRoad ( const Coordinates& coord, unsigned servingDistance ) const;

public:
    /**
     * Définit les parcelles dans le voisinage d'une route comme étant exploitables
     * @param servingDistance Distance maximale du voisinage
     */
    void defineUsables(unsigned servingDistance);

};

#endif //FIELD_H
