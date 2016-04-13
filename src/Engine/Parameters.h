#pragma once

class Parameters
{
private:
	unsigned road_width;
	unsigned serve_distance;

public:
	Parameters(unsigned _road_width, unsigned _serve_distance);
	~Parameters();
        
        inline unsigned get_road_width() const
            { return road_width; }
        inline unsigned get_serve_distance() const
            { return serve_distance; }
        
};

