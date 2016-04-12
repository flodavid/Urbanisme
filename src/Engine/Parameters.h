#pragma once

class Parameters
{
private:
	unsigned road_width;
	unsigned serve_distance;

public:
	Parameters(unsigned _road_width, unsigned _serve_distance);
	~Parameters();
        
        unsigned get_road_width() const
            { return road_width; }
        unsigned get_serve_distance() const
            { return serve_distance; }
        
};

