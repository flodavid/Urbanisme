#pragma once

/**
 * Représente les paramètres du problèmes
 */
class Parameters
{
private:
    unsigned serve_distance;
    unsigned road_width;

public:
    /**
     * Constructeur de la classe paramètres
     * @param _road_width Largeur des routes
     * @param _serve_distance Distance de desserte des routes
     */
    Parameters(unsigned _serve_distance, unsigned _road_width);
    /**
     * Constructeur par recopie de la classe
     * @param other Instance à recopier afin de créer la nouvelle instance
     */
    Parameters(const Parameters& other);
    ~Parameters();

    /**
     * Accesseur sur la largeur de la route
     * @return La largeur de la route, unsigned int
     */
    inline unsigned get_road_width() const
    { return road_width; }
    /**
     * Accesseur sur la distance à laquelle les parcelles sont desservies
     * @return La distance de desserte, unsigned int
     */
    inline unsigned get_serve_distance() const
    { return serve_distance; }

    /**
     * Mutateur sur la largeur de la route
     * @param width La nouvelle largeur de la route, unsigned int
     */
    inline void set_road_width(unsigned width)
    { road_width= width; }
    /**
     * Mutateur sur la distance à laquelle les parcelles sont desservies
     * @param distance La nouvelle distance de desserte, unsigned int
     */
    inline void set_serve_distance(unsigned distance)
    { serve_distance= distance; }

};
