#pragma once
#include "Field.h"
#include "Parameters.h"

class Resolution
{
private:
	Field field;
	Parameters params;

public:
	Resolution(const Field & _field, const Parameters & _params);
	~Resolution();
};

