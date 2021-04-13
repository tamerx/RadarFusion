#include "Coordinate.h"

Coordinate::Coordinate()
{
}

Coordinate::Coordinate(float latitude, float longitude)
{
	lat = latitude;
	lon = longitude;
}

void Coordinate::set(float latitude, float longitude)
{
	lat = latitude;
	lon = longitude;
}
