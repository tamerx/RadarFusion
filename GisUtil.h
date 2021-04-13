#pragma once
#include <math.h>
#include <vector>


#include "Coordinate.h"

class GisUtil
{


public:
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define VINCENTYS_MAX_STEPS 1000

	const int POLYGON_CLIPPING_MAX_POINTS = 100;


	// This function is used to calculate the latitude and longitude of second point with
	// latitude, longitude of reference point, bearing (degrees) and distance (meters).
	// It returns the latitude and longitude of calculated point.
	static Coordinate calculateP2FromP1BearingDistance(Coordinate p1, double bearing, double distance);



	//#define VINCENTYS_TIME_CONSTANT 10000


/*
	All distance algorithms in this file;
	Takes latitudes (double) and longtitudes (double) of 2 points as degrees,
	And returns the distance (double) between them in meters.
*/

	double haversineDistanceFormula(double lat1, double lon1, double lat2, double lon2);

	double sphericalLawOfCosinus(double lat1, double lon1, double lat2, double lon2);
	
	double vincentysInverseFormula(double lat1, double lon1, double lat2, double lon2);

	double haversineDistanceFormula2(double lat1, double lon1, double lat2, double lon2);

	// Takes latitudes and longitudes of 2 points (reference and target) in degrees;
	// psio is the angular direction of flat Earth x-axis (degrees clockwise from north),
	// which is the angle in degrees used for converting flat Earth x and y coordinates to the North and East coordinates;
	// and href is the reference height from the surface of the Earth to the flat Earth frame with regard to the flat Earth frame, in meters.
	// This function is implemented from matlab (kind of) and returns d (distance) between 2 given points in meters.
	static double lla2flatDistanceFormula(double lat1, double lon1, double lat2, double lon2);


	// This function checks if the given point is inside of given polygon,
	// takes point q (as Coordinate) and polygon points (as Coordinate vector),
	// Returns true or false (boolean)
	static bool isPointInsidePolygon(Coordinate q, std::vector<Coordinate> points);


	// Sutherland–Hodgman algorithm for polygon clipping
	// Used in boundary calculation of two radar sectors

	

	// Returns x-value of point of intersection of two lines
	double CLIPPING_x_intersect(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4);

	// Returns y-value of point of intersection of two lines
	double CLIPPING_y_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);

	// This functions clips all the edges w.r.t one clip
	// edge of clipping area
	void CLIPPING_clip(double poly_points[][2], int &poly_size, double x1, double y1, double x2, double y2);

	// Sutherland–Hodgman algorithm:
	// Takes points of subject and clipper polygons
	// (points MUST be added in order to clockwise) (as Coordinate vectors),
	// Returns the clipped polygons points (as Coordinate vector)
	std::vector<Coordinate> sutherlandHodgmanClip(std::vector<Coordinate> subject_points_, std::vector<Coordinate> clipper_points_);

	void printPolygon(std::vector<Coordinate> poly);





};

