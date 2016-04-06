#pragma once
#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Engine/coordinates.h"

class Resolution
{
private:
	Field field;
	Parameters params;

public:
	Resolution(const Field & _field, const Parameters & _params);
	~Resolution();
	
/* Getters */
	
/* Setters */
	void setParams(const Parameters& _params);
    
/* Evaluations */
	/**
	 * Compte le nombre total de parcelles exploitables sur la surface
	 * @return nombre de parcelles exploitables
	 */
	unsigned evaluateTotalUsable() const;
	/**
	 * Calcule le ratio entre distance par la route et distance Manhattan entre deux positions
	 * @param p1 premières coordonnées
	 * @param p2 secondes coordonnées
	 * @return le ratio, sous forme de flottant
	 */
	float manhattanRatioBetween2Parcels(const Coordinates& p1, const Coordinates& p2) const;
	float evaluateRatio() const;
};

