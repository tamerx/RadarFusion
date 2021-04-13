#ifndef COORDINATE
#define COORDINATE


class Coordinate {
public:
    float lat;
    float lon;
    
	Coordinate();
    
	Coordinate(float latitude, float longitude);
    
	void set(float latitude, float longitude);
};

#endif