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
	
	unsigned evaluateTotalUsable() const;
	float ratioBetweenParcels(const Coordinates& p1, const Coordinates& p2) const;
	float evaluateRatio() const;
};

