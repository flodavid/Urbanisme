#pragma once

class Parameters
{
private:
	unsigned road_width;
	unsigned serve_distance;

public:
	Parameters(unsigned _road_width, unsigned _serve_distance);
	~Parameters();
};

