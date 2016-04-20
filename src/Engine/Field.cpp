#include "../stdafx.h"
#include "Field.h"

using namespace std;

Field::Field ( unsigned width, unsigned height/*, std::list<Coordinates>& inputs_and_ouputs*/ ) :
    nb_cols ( width ), nb_rows ( height ) /*, ins_outs(inputs_and_ouputs)*/
{
    resizeWithDimensions();
}

Field::~Field()
{
}

///#######################
///      Setters
///#######################
//@{

void Field::resizeWithDimensions()
{
    parcels.resize ( nb_cols );

    for ( vector<State>& parcel_row : parcels ) {
        parcel_row.resize ( nb_cols, is_undefined );
    }
}

//@}
///###################
///      Affichage
///###################
//@{


void Field::show_ins_and_outs() const
{
    for ( const Coordinates& in_or_out : ins_outs ) {
        cout << in_or_out << " ; ";
    }
    cout << endl;
}

void Field::show_states() const
{
    Coordinates coord= Field::first();
    do {
        if ( ( coord.col ) == 0 ) {
            cout << endl<< coord<< "= "<< at ( coord );
        } else {
            cout<<  "; "<< coord<< "= "<< at ( coord );
        }
    } while ( nextCoordinates ( &coord ) );
}

//@}
///########################################
///=== Fonctions sur les coordonnées ===///
///########################################
//@{

bool Field::contains ( int x, int y ) const
{
    return ( x >= 0 && ( ( unsigned ) x ) < nb_cols ) // abscisse correcte
           && ( y >= 0  && ( ( unsigned ) y ) < nb_rows ); // ordonnée correcte
}


bool Field::contains ( const Coordinates& coord ) const
{
    return contains ( coord.col, coord.row );
}

bool Field::nextCoordinates ( Coordinates* coord ) const
{
    // On vérifie que la coordonnée actuelle est dans la surface
    //  (vérification sur les colonnes puis sur les lignes)
    if ( ( coord->col < -1 || coord->col >= ( int ) ( get_width() ) ) ||
            ( coord->row < 0 || coord->row >= ( int ) ( get_height() ) ) ) {
#if DEBUG_PARCOURS_COORDS
        cout << "Coordonnées "<< ( *coord ) << " en dehors de la surface" <<endl;
#endif
        return false;
    } else {
        // Si on est à la fin de la ligne, on passe à la ligne suivante
        if ( ( unsigned ) ( coord->col ) +1 == get_width() ) {
            // seulement si on n'est pas à la fin de la surface
            if ( ( unsigned ) ( coord->row ) +1 < get_width() ) {
                coord->col= 0;
                coord->row+= 1;
#if DEBUG_PARCOURS_COORDS
                cout << "Passage à la ligne suivante" <<endl;
#endif
                return true;
            } else {
                // dernière case de la surface
#if DEBUG_PARCOURS_COORDS
                cout << "Dernière case de la matrice" <<endl;
#endif
                return false;
            }
        }// Sinon, on se décale sur la ligne
        else {
            coord->col+= 1;
#if DEBUG_PARCOURS_COORDS
            cout << "Passage à la case suivante (à droite)" <<endl;
#endif
            return true;
        }
    }
}

//@}
///#############################
///===  Méthodes générales	===/
///#############################

void Field::generateInsAndOuts ( unsigned nb )
{
    bool on_top_or_down;
    for ( unsigned num_in_out = 0; num_in_out < nb; ++num_in_out ) {
        on_top_or_down = ( bool ) ( rand() % 2 );
        int row, col;

        // On choisit une case en haut ou en bas, avec une colonne aléatoire
        if ( on_top_or_down ) {
            row = ( rand() % 2 ) * nb_rows;
            col = rand() % nb_cols;
        }
        // Même opération en inversant lignes et colonnes
        else {
            row = rand() % nb_rows;
            col = ( rand() % 2 ) * nb_cols;
        }

        ins_outs.push_back ( Coordinates ( col, row ) );
    }
}

bool Field::isRoadAndNeighbourOf (const Coordinates& neighbour, const Coordinates& coord, unsigned servingDistance ) const
{
    return contains ( neighbour )
           && ! ( neighbour == coord )
           && coord.manhattanDistance ( neighbour ) <= servingDistance // TODO changer, ne pas utiliser manhattanDistance,  peu performant ?
           && at ( neighbour ) == is_road;
}

std::list<Coordinates>* Field::getNeighbourRoads ( const Coordinates& coord ) const
{
#if DEBUG_ROADS_DIST
    cout << "Recherche des voisins de la parcelle en "<< coord.col<< " ; "<< coord.row<< endl;
#endif
    list<Coordinates>* neighbour_roads= new list<Coordinates>;

    Coordinates& west= * ( new Coordinates ( coord.col -1, coord.row ) );
    Coordinates& east= * ( new Coordinates ( coord.col +1, coord.row ) );
    Coordinates& north= * ( new Coordinates ( coord.col, coord.row -1 ) );
    Coordinates& south= * ( new Coordinates ( coord.col, coord.row +1 ) );
// On vérifie que chaque voisin n'est pas en dehors de la matrice

    if ( contains ( west ) && at ( west ) == is_road ) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back ( west );
#if DEBUG_ROADS_DIST
        cout << "\tparcelle "<< west<< endl;
#endif
    }
    if ( contains ( east ) && at ( east ) == is_road ) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back ( east );
#if DEBUG_ROADS_DIST
        cout << "\tparcelle "<< east<< endl;
#endif
    }
    if ( contains ( north ) && at ( north ) == is_road ) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back ( north );
#if DEBUG_ROADS_DIST
        cout << "\tparcelle "<< north<< endl;
#endif
    }
    if ( contains ( south ) && at ( south ) == is_road ) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back ( south );
#if DEBUG_ROADS_DIST
        cout << "\tparcelle "<< south<< endl;
#endif
    }

    /// @see améliorer les listes, utiliser pointeurs ou non ?
    delete &north;
    delete &south;
    delete &west;
    delete &east;

    return neighbour_roads;
}

std::list<Coordinates>* Field::getServingRoads ( const Coordinates& coord , unsigned servingDistance ) const
{
    list<Coordinates>* serving_roads= new list<Coordinates>;

    int serve_dist= ( int ) servingDistance; // il est plus simple de convertir en entier

    // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
    /// @see on vérifie serve_dist² parcelles,  alors qu'on pourrait en vérifier ?? (moins)
    for ( int i= coord.row + serve_dist; i >= coord.row -serve_dist; --i ) {
        for ( int j= coord.col + serve_dist; j >= coord.col -serve_dist; --j ) {

            // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            Coordinates neighbour ( j,  i );
            if ( isRoadAndNeighbourOf ( neighbour, coord, servingDistance ) ) {
                // Ajout dans les routes voisines de la parcelle
                Coordinates& road_coord= * ( new Coordinates ( j, i ) );
                serving_roads->push_back ( road_coord );    //  copie faire :'(
                delete &road_coord; // @SEE utilisation pointeur ou non
#if DEBUG_ROADS_DIST
                cout << "parcelle en "<< j<< " ; "<< i<< " est une route voisine de la parcelle en "
                     << coord.col<< " ; "<< coord.row<< endl;
#endif
            }
        }
    }

    return serving_roads;
}

bool Field::hasServingRoad ( const Coordinates& coord , unsigned servingDistance ) const
{
    int serve_dist= ( int ) servingDistance; // il est plus simple de convertir en entier

    // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
    /// @see on vérifie serve_dist² parcelles,  alors qu'on pourrait en vérifier ?? (moins)
    for ( int i= coord.row + serve_dist; i >= coord.row -serve_dist; --i ) {
        for ( int j= coord.col + serve_dist; j >= coord.col -serve_dist; --j ) {

            // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            Coordinates neighbour ( j,  i );
            if ( isRoadAndNeighbourOf ( neighbour,  coord,  serve_dist ) ) {
#if DEBUG_ROADS_DIST
                cout << "parcelle en "<< j<< " ; "<< i<< " est une route voisine de la parcelle en "
                     << coord.col<< " ; "<< coord.row<< endl;
#endif
                // SORT DE LA DOUBLE BOUCLE, on a trouvé un voisin
                return true;                                
            }
        }
    }

    // Si on arrive à ce point, c'est que l'on n'a pas trouvé de voisin
    return false;
}

void Field::defineUsables ( unsigned int servingDistance )
{
    /// @see je vérifie que chaque route a un voisin (en cherchant tous ses voisins,
    //  alors qu'on pourrait s'arrêter au premier), l'inverse est possible :
    //  définir toutes les voisins de chaque route comme étant exploitable
    Coordinates& coord= first();
    do {
#if DEBUG
        if ( ( coord.col ) == 0 ) {
            cout << endl<< coord;
        } else {
            cout<<  "; "<< coord;
        }
#endif
        if ( at ( coord ) == is_undefined ) {
            if ( hasServingRoad(coord, servingDistance) ) {
                parcels[coord.row][coord.col] = is_usable;
            } else {
                parcels[coord.row][coord.col] = is_unusable;
            }
        }
    } while ( nextCoordinates ( &coord ) );
    delete &coord;
}


//@}
///#################################
///===  Recherche de solutions	===/
///#################################
//@{
void Field::generateRandomSolution()
{
    Coordinates& coord= first();
//     int road_percent = 20;
    
    do {
//         maybePlaceRoad(coord, road_percent);
    } while ( nextCoordinates ( &coord ) );
}
    
//@}
