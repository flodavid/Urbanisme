#pragma once

#include <list>
#include <vector>

#include "coordinates.h"

enum State{is_undefined= -1, is_usable, is_unusable, is_road, is_in_out, max_state};

/**
 * @brief Classe représentant la surface -le terrain- et contenant les opérations que l'on
 * effectue sur celui-ci. L'état d'une instance de cette classe à la fin de l'algorithme
 * représente une solution trouvée après la recherche.
 * On peut donc avoir plusieurs Field lors d'une exécution.
 */
class Field
{

private:
    unsigned nb_cols, nb_rows;
    std::list<Coordinates> ins_outs;

    std::vector<std::vector<State>> parcels;

public:
    /**
     * Constructeur de la classe
     * @param width Largeur à définir de la surface
     * @param height Hauteur à définir de la surface
     */
    Field(unsigned width, unsigned height/*, std::list< Coordinates >& inputs_and_ouputs = *(new std::list<Coordinates>()) */);
    /**
     * Constructeur par recopie de la classe Field
     * @param other Instance à recopier
     */
    Field(const Field& other);
    ~Field();

    /*=== Getters ===*/
    /**
     * Accesseur sur le nombre de colonnes -la largeur- de la surface
     * @return La largeur de la surface
     */
    inline unsigned get_width() const	{ return nb_cols; }
    /**
     * Accesseur sur le nombre de lignes -la hauteur- de la surface
     * @return La hauteur de la surface
     */
    inline unsigned get_height() const	{ return nb_rows; }
    
    /**
     * Accesseur sur la liste des entrées et sorties
     * @return
     */
    inline std::list<Coordinates> & get_insOuts() { return ins_outs; }
 
    /**
     * Donne le nombre de parcelles de la surface
     * @return Le nombre de parcelles, nombre de ligne facteur nombre de colonnes,
     * un entier non signé
     */
    inline unsigned getNbParcels() const	{ return nb_cols * nb_rows; }

    /*=== Setters ===*/
    /**
     * Mutateur sur le nombre de colonnes -la largeur- de la surface
     * @param width La nouvelle largeur de la surface
     */
    inline void set_width(unsigned width)
    { nb_cols =  width; }
    /**
     * Mutateur sur le nombre de lignes -la hauteur- de la surface
     * @param height La nouvelle hauteur de la surface
     */
    inline void set_height(unsigned height)
    { nb_rows =  height; }

    //private:
    /**
     * Redéfinit la taille du vecteur de vecteur d'états selon le nombre de lignes
     * et de colonnes contenues dans la classe. On utilise la fonction resize().
     * Attention, les données contenues dans le vecteur peuvent être modifiées
     */
    void resizeWithDimensions();
public:
    /**
     * Ajoute une route à partir de coordonnées
     * @param coords Coordonnées de la future route
     */
    void add_road(const Coordinates& coords)
    { parcels[coords.row][coords.col]= is_road; }

    /**
     * Ajoute une route à partir de coordonnées
     * @param col Colonne de la route
     * @param row Ligne de la route
     */
    void add_road(unsigned col, unsigned row)
    { parcels[row][col]= is_road; }
    /**
     * Ajoute une entrée/sortie à partir de coordonnées
     * @param coords Coordonnées de l'E/S
     */
    bool tryAdd_in_out(const Coordinates& coords);
    /**
     * Ajoute une entrée/sortie à partir de coordonnées
     * @param col Colonne de l'E/S
     * @param row Ligne de l'E/S
     */
    bool tryAdd_in_out(unsigned col, unsigned row)
    { return tryAdd_in_out(Coordinates(col, row)); }

    /**
     * Supprime l'état de la parcelle
     * @param coords
     */
    void add_undefined(const Coordinates& coords);

    /**
     * Supprime l'état de la parcelle
     * @param col
     * @param row
     */
    void add_undefined(unsigned col, unsigned row)
    { add_undefined(Coordinates(col, row)); }

    /**=== Operators ===**/
public:
    /**
     * Retourne la parcelle de la surface aux coordonnées passées en paramètre
     * @param pos Coordonnées de la parcelle
     * @return L'état de la parcelle à la position des coordonnées, un State
     */
    inline State at(const Coordinates& pos) const { return parcels[pos.row][pos.col]; }
    /**
     * Opérateur [] avec des coordonnées sur la surface,
     * retourne la parcelle de la surface aux coordonnées passées en paramètre
     * @param pos Coordonnées de la parcelle
     * @return L'état de la parcelle à la position des coordonnées, un State
     */
    inline State operator[](const Coordinates& pos) const { return parcels[pos.row][pos.col]; }
    /**
     * Opérateur = d'affectation
     * @param other Instance à recopier
     */
    inline void operator=(const Field& other)
    {
        assert(nb_cols == other.nb_cols && nb_rows == other.nb_cols);
        ins_outs= other.ins_outs;
        parcels= other.parcels;
    }

    /**
     * @brief begin
     * @return un itérateur constant sur la première parcelle du Field
     * @see
     */
    inline std::vector<std::vector<State>>::const_iterator begin() const { return parcels.cbegin(); }
    /**
     * @brief end
     * @return un itérateur constant sur la dernière parcelle du Field
     * @see
     */
    inline std::vector<std::vector<State>>::const_iterator end() const { return parcels.cend(); }
    /**
     * @brief begin
     * @return un itérateur sur la première parcelle du Field
     * @see
     */
    inline std::vector<std::vector<State>>::iterator begin() { return parcels.begin(); }
    /**
     * @brief end
     * @return un itérateur sur la dernière parcelle du Field
     * @see
     */
    inline std::vector<std::vector<State>>::iterator end() { return parcels.end(); }

    /**
     * Retourne la première coordonée de la matrice, elle peut ensuite
     * être modifée pour servir d'itérateur à l'aide de nextCoordinates()
     * @return une référence, non constante, sur la première coordonnée de la matrice
     */
    static inline Coordinates& first()
    { return *(new Coordinates(0,0)); }

public:
    /*=== Affichage ===*/
    /**
     * Impression sur la sortie standard des entrées et sorties de la surface
     */
    void show_ins_and_outs() const;

    /**
     * Imprime dans un flux les informations de l'instance
     * @param out Flux dans lequel on imprime les informations
     * @return Le flux en entrée sur lequel on a imprimé les informations
     */
    std::ostream& print(std::ostream& out) const;
    /**
     * Surcharge de l'opérateur << de sortie sur flux
     * @param out Flux dans lequel on imprime les informations
     * @param field Surface que l'on souhaite imprimer
     * @return  Le flux en entrée sur lequel on a imprimé les informations de la
     * surface passée en entrée
     */
    friend std::ostream& operator<<(std::ostream& out, const Field& field)
    { return field.print(out); }

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
     * Vérifie qu'une position est valide et a une route
        * @param neighbour Coordonnées de la cellule
     * @return vrai si la cellule est une route
     */
    inline bool isRoad(const Coordinates& neighbour) const;
    /**
     * Vérifie qu'une position est valide et a une parcelle exploitable
        * @param neighbour Coordonnées de la parcelle
     * @return vrai si la parcelle est exploitable
     */
    inline bool isParcel(const Coordinates &neighbour) const;
    /**
     * Vérifie qu'une position est valide et a une parcelle non exploitable
        * @param neighbour Coordonnées de la parcelle
     * @return vrai si la parcelle n'est pas exploitable
     */
    inline bool isUnusableParcel(const Coordinates &neighbour) const;
    /**
     * Vérifie que deux parcelles sont voisines, supposant une distance de voisinage
        * @param neighbour Coordonnées de la parcelle qui peut être voisine de l'autre
        * @param coord Coordonnées de la seconde parcelle
        * @param servingDistance Distance de desserte des routes
     * @return vrai si la première parcelle est une route, voisine de la seconde
     */
    bool isRoadAndNeighbourOf(const Coordinates& neighbour, const Coordinates& coord, unsigned servingDistance) const;

public:
//    /**
//     * Recherche des parcelles qui sont concomitantes à la cellule
//        * @param coord Coordonnées de la parcelle
//     * @return une liste de parcelle adjacentes à la cellule
//     */
//    std::list<Coordinates> *getNeighbourParcels( const Coordinates& coord ) const;
    /**
     * Recherche des portions de routes qui sont concomitantes à la parcelle courante
        * @param coord Coordonnées de la parcelle
     * @return une liste de routes adjacentes à la parcelle
     */
    std::list<Coordinates> *getNeighbourRoads( const Coordinates& coord ) const;


// bool
private:
    std::list<Coordinates> *getClose(const Coordinates &coord, unsigned maxDist, bool (Field::*neighbourVerif)(const Coordinates &) const) const;

public:
    /**
     * Recherche des parcelles qui serait desservies grâce au passage de la parcelle en route
        * @param coord Coordonnées de la parcelle
        * @param servingDistance Distance à laquelle les routes rendent les parcelles utilisables
     * @return une liste de parcelles qui deviendraient utilisables si la parcelle devenait une route
     */
    std::list<Coordinates> *getCloseUnusableParcels( const Coordinates& coord, unsigned servingDistance ) const;
    /**
     * Recherche des parcelles qui sont proches de la position
     * @param coord Coordonnées de la position
     * @param maxDist Distance maximale entre la parcelle et la position
     * @return une liste de parcelles proches de la position donnée
     */
    std::list<Coordinates> *getCloseParcels( const Coordinates& coord, unsigned maxDist ) const;
    /**
     * Recherche des routes qui sont proches de la position
     * @param coord Coordonnées de la position
     * @param maxDist Distance maximale entre la route et la position
     * @return une liste de routes proches de la position donnée
     */
    std::list<Coordinates> *getCloseRoads( const Coordinates& coord, unsigned maxDist ) const;
    /**
     * Recherche des portions de routes qui peuvent désservir la parcelle
         * @param coord Coordonnées de la parcelle à desservir
         * @param servingDistance Distance maximale à laquelle un route peut desservir une parcelle
     * @return une liste de routes distance inférieure ou égale à la distance de desserte
     */
    std::list<Coordinates> *getServingRoads( const Coordinates& coord, unsigned servingDistance ) const;

//private:
    /**
    * Vérifie que la parcelle a un voisin
        * @param coord Coordonnées de la parcelle à desservir
        * @param servingDistance Distance de desserte des routes
    * @return vrai si la parcelle est desservie par au moins une route
    */
    bool hasServingRoad ( const Coordinates& coord, unsigned servingDistance ) const;

public:
    /**
     * Définit les parcelles dans le voisinage d'une route comme étant exploitables
        * @param servingDistance Distance maximale du voisinage
     */
    void defineUsables(unsigned servingDistance);
    /**
     * Met à jour les parcelles dans le voisinage d'une route comme étant exploitables
        * @param servingDistance Distance maximale du voisinage
     */
    void updateUsables(unsigned servingDistance);
    
    /*=== Autres méthodes utiles ===*/
    /**
     * @brief createExample
     * @test Utile seulement pour les tests
     */
    void createExample();
};
