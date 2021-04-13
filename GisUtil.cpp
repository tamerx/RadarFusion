#include "GisUtil.h"


 Coordinate GisUtil::calculateP2FromP1BearingDistance(Coordinate p1, double bearing, double distance) {
	const double R = 6371e3;
	const double distance_rad = distance / R;
	const double theta = bearing * M_PI / 180;
	const double lat1 = p1.lat * M_PI / 180;
	const double lon1 = p1.lon * M_PI / 180;

	double lat2 = asin(sin(lat1) * cos(distance_rad) + cos(lat1) * sin(distance_rad) * cos(theta));
	double lon2 = lon1 + atan2(sin(theta) * sin(distance_rad) * cos(lat1), cos(distance_rad) - sin(lat1) * sin(lat2));

	// TODO (later) : double % double ???
	// normalise to -180..+180°
	//lon2 = (lon2 + 3 * M_PI) % (2 * M_PI) - M_PI;

	// convert them to degrees
	lat2 = lat2 * 180 / M_PI;
	lon2 = lon2 * 180 / M_PI;

	return Coordinate(lat2, lon2);
}

double GisUtil::sphericalLawOfCosinus(double lat1, double lon1, double lat2, double lon2)
{

	const double R = 6371e3; // in meters
	const double phi_1 = lat1 * M_PI / 180;
	const double phi_2 = lat2 * M_PI / 180;
	//const double delta_phi = abs(lat2 - lat1) * M_PI / 180;
	const double delta_lambda = abs(lon2 - lon1) * M_PI / 180;

	/*
	double a = sin(phi_1) * sin(phi_2);
	double b = cos(phi_1) * cos(phi_2) * cos(delta_lambda);
	double d = acos(a + b) * R;
	*/
	double d = acos(sin(phi_1) * sin(phi_2) + cos(phi_1) * cos(phi_2) * cos(delta_lambda)) * R;

	return d;
}

double GisUtil::vincentysInverseFormula(double lat1, double lon1, double lat2, double lon2)
{

	// get constants
	const double a = 6378137.0; // metres in WGS-84
	const double f = 1 / 298.257223563; // in WGS-84
	const double b = (1 - f) * a; // 6356752.314245 // meters in WGS-84

	// get parameters as radians
	const double phi1 = lat1 * M_PI / 180;
	const double L1 = lon1 * M_PI / 180;
	const double phi2 = lat2 * M_PI / 180;
	const double L2 = lon2 * M_PI / 180;

	const double U1 = atan((1 - f) * tan(phi1));
	const double U2 = atan((1 - f) * tan(phi2));

	const double L = L2 - L1;
	double lambda = L, previous_lambda;

	int counter = 0; // robustness
	double sin_lambda, cos_lambda, sin_sigma, cos_sigma, sin_alpha, cos2_alpha, cos_2sigma_m, cos2_2sigma_m, C, sigma, sqr_u, A, B, sin2_sigma, delta_sigma, distance;

	// iterative evaluation
	do {
		previous_lambda = lambda; // (13)
		sin_lambda = sin(lambda);
		cos_lambda = cos(lambda);
		sin_sigma = sqrt(pow((cos(U2) * sin(lambda)), 2) + pow((cos(U1) * sin(U2) - sin(U1) * cos(U2) * cos(lambda)), 2)); // (14)
		cos_sigma = sin(U1) * sin(U2) + cos(U1) * cos(U2) * cos(lambda); // (15)
		sigma = atan2(sin_sigma, cos_sigma); // (16)
		sin_alpha = cos(U1) * cos(U2) * sin_lambda / sin_sigma; // (17)
		cos2_alpha = 1 - pow((sin_alpha), 2);
		cos_2sigma_m = (cos2_alpha == 0) ? 0 : cos_sigma - 2 * sin(U1) * sin(U2) / cos2_alpha; // (18)
		cos2_2sigma_m = pow((cos_2sigma_m), 2);

		C = f / 16 * cos2_alpha * (4 + f * (4 - 3 * cos2_alpha)); // (10)

		lambda = L + (1 - C) * f * sin_alpha *
			(sigma + C * sin_sigma * (cos_2sigma_m + C * cos_sigma * (-1 + 2 * cos2_2sigma_m))); // (11)

		++counter; // robustness

	} while (abs(previous_lambda - lambda) > 1e-12
		&& abs(lambda) < M_PI
		&& counter < VINCENTYS_MAX_STEPS); // robustness

	sqr_u = cos2_alpha * ((pow(a, 2) - pow(b, 2)) / pow(b, 2)); // above (1)

	A = 1 + (sqr_u / 16384) * (4096 + sqr_u * (-768 + sqr_u * (320 - 175 * sqr_u))); // (3)
	B = (sqr_u / 1024) * (256 + sqr_u * (-128 + sqr_u * (74 - 47 * sqr_u))); // (4)
	cos_sigma = cos(sigma);
	sin2_sigma = pow((sin_sigma), 2);
	delta_sigma = B * sin_sigma * (cos_2sigma_m + (B / 4) * (cos_sigma* (-1 + 2 * cos2_2sigma_m)
		- (B / 6) * cos_2sigma_m * (-3 + 4 * sin2_sigma) * (-3 + 4 * cos2_2sigma_m))); // (6)

	distance = b * A * (sigma - delta_sigma); // (19)

	return distance;
}

double GisUtil::haversineDistanceFormula2(double lat1, double lon1, double lat2, double lon2)
{
	const double R = 6371e3; // in meters
	const double phi_1 = lat1 * M_PI / 180;
	const double phi_2 = lat2 * M_PI / 180;

	const double lambda_1 = lon1 * M_PI / 180;
	const double lambda_2 = lon2 * M_PI / 180;

	const double delta_phi = abs(lat2 - lat1) * M_PI / 180;
	const double delta_lambda = abs(lon2 - lon1) * M_PI / 180;

	double a = pow(sin(delta_phi / 2), 2);
	double b = cos(phi_1) * cos(phi_2) * pow(sin(delta_lambda / 2), 2);

	double d = 2 * R * asin(sqrt(a + b));

	return d;
}

double GisUtil::lla2flatDistanceFormula(double lat1, double lon1, double lat2, double lon2)
{
	double psio = 30;
	double href = 0;

	const double R = 6378137;
	const double f = 0.00335281066474748071;

	// target
	const double lat_p = lat1 * M_PI / 180;
	const double lon_p = lon1 * M_PI / 180;
	const double alt_p = href;

	// reference
	const double lat_o = lat2 * M_PI / 180;
	const double lon_o = lon2 * M_PI / 180;

	psio = psio * M_PI / 180;

	double d_lat = abs(lat_p - lat_o);
	double d_lon = abs(lon_p - lon_o);

	double ff = (2.0 * f) - pow(f, 2);
	double sin_lat = sin(lat_o);

	double Rn = R / sqrt(1 - (ff * pow(sin_lat, 2)));
	double Rm = Rn * (1 - ff) / (1 - ff * pow(sin_lat, 2));


	double d_north = d_lat / atan2(1, Rm);
	double d_east = d_lon / atan2(1, (Rn * cos(lat_o)));

	/*
	 double d_north = d_lat / atan(1 / Rm);
	 double d_east = d_lon / atan(1 / (Rn * cos(lat_o)));
	 */

	 // Rotate matrix clockwise
	double x_p = (d_north * cos(psio)) + d_east * sin(psio);
	double y_p = (-d_north * sin(psio)) + d_east * cos(psio);
	double z_p = -alt_p - href;

	double w_p = sqrt(pow(x_p, 2) + pow(y_p, 2));



	return w_p;
}

bool GisUtil::isPointInsidePolygon(Coordinate q, std::vector<Coordinate> points)
{
	bool result = false;

	double pi[2];
	double pj[2];

	// Jordan Curve Theorem
	for (int i = 0, j = points.size() - 1; i < points.size(); j = i++)
	{
		pi[0] = points[i].lat;
		pi[1] = points[i].lon;
		pj[0] = points[j].lat;
		pj[1] = points[j].lon;

		if (((pi[1] > q.lon) != (pj[1] > q.lon)) && (q.lat < (pj[0] - pi[0]) * (q.lon - pi[1]) / (pj[1] - pi[1]) + pi[0]))
		{
			result = !result;
		}
	}

	return result;
}

double GisUtil::CLIPPING_x_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	double num = (x1*y2 - y1 * x2) * (x3 - x4) -
		(x1 - x2) * (x3*y4 - y3 * x4);
	double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	return num / den;
}

double GisUtil::CLIPPING_y_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	double num = (x1*y2 - y1 * x2) * (y3 - y4) -
		(y1 - y2) * (x3*y4 - y3 * x4);
	double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	return num / den;
}

void GisUtil::CLIPPING_clip(double poly_points[][2], int & poly_size, double x1, double y1, double x2, double y2)
{
	double new_points[100][2];
	int new_poly_size = 0;

	// (ix,iy),(kx,ky) are the coordinate values of
	// the points
	for (int i = 0; i < poly_size; i++) {
		// i and k form a line in polygon
		int k = (i + 1) % poly_size;
		double ix = poly_points[i][0], iy = poly_points[i][1];
		double kx = poly_points[k][0], ky = poly_points[k][1];

		// Calculating position of first point
		// w.r.t. clipper line
		double i_pos = (x2 - x1) * (iy - y1) - (y2 - y1) * (ix - x1);

		// Calculating position of second point
		// w.r.t. clipper line
		double k_pos = (x2 - x1) * (ky - y1) - (y2 - y1) * (kx - x1);

		// Case 1 : When both points are inside
		if (i_pos < 0 && k_pos < 0) {
			//Only second point is added
			new_points[new_poly_size][0] = kx;
			new_points[new_poly_size][1] = ky;
			new_poly_size++;
		}

		// Case 2: When only first point is outside
		else if (i_pos >= 0 && k_pos < 0) {
			// Point of intersection with edge
			// and the second point is added
			new_points[new_poly_size][0] = CLIPPING_x_intersect(x1,
				y1, x2, y2, ix, iy, kx, ky);
			new_points[new_poly_size][1] = CLIPPING_y_intersect(x1,
				y1, x2, y2, ix, iy, kx, ky);
			new_poly_size++;

			new_points[new_poly_size][0] = kx;
			new_points[new_poly_size][1] = ky;
			new_poly_size++;
		}

		// Case 3: When only second point is outside
		else if (i_pos < 0 && k_pos >= 0) {
			//Only point of intersection with edge is added
			new_points[new_poly_size][0] = CLIPPING_x_intersect(x1,
				y1, x2, y2, ix, iy, kx, ky);
			new_points[new_poly_size][1] = CLIPPING_y_intersect(x1,
				y1, x2, y2, ix, iy, kx, ky);
			new_poly_size++;
		}

		// Case 4: When both points are outside
		else {
			//No points are added
		}
	}

	// Copying new points into original array
	// and changing the number of vertices
	poly_size = new_poly_size;
	for (int i = 0; i < poly_size; i++) {
		poly_points[i][0] = new_points[i][0];
		poly_points[i][1] = new_points[i][1];
	}
}

std::vector<Coordinate> GisUtil::sutherlandHodgmanClip(std::vector<Coordinate> subject_points_, std::vector<Coordinate> clipper_points_)
{
	int poly_size = subject_points_.size();
	int clipper_size = clipper_points_.size();

	double poly_points[100][2];
	double clipper_points[100][2];

	for (int i = 0; i < subject_points_.size(); i++) {
		poly_points[i][0] = subject_points_[i].lon;
		poly_points[i][1] = subject_points_[i].lat;
	}
	for (int i = 0; i < clipper_size; i++) {
		clipper_points[i][0] = clipper_points_[i].lon;
		clipper_points[i][1] = clipper_points_[i].lat;
	}

	//i and k are two consecutive indexes
	for (int i = 0; i < clipper_size; i++) {
		int k = (i + 1) % clipper_size;

		// We pass the current array of vertices, it's size
		// and the end points of the selected clipper line
		CLIPPING_clip(poly_points, poly_size, clipper_points[i][0],
			clipper_points[i][1], clipper_points[k][0],
			clipper_points[k][1]);
	}


	// Printing vertices of clipped polygon
	std::vector<Coordinate> boundary_polygon;
	for (int i = 0; i < poly_size; i++)
		//printf("(%f, %f)\n", poly_points[i][0], poly_points[i][1]);
		boundary_polygon.push_back(Coordinate(poly_points[i][1], poly_points[i][0]));


	return boundary_polygon;
}

void GisUtil::printPolygon(std::vector<Coordinate> poly)
{
	for (int i = 0; i < poly.size(); i++)
		printf("%f, %f\n", poly[i].lat, poly[i].lon);
	printf("\n");
}

double GisUtil::haversineDistanceFormula(double lat1, double lon1, double lat2, double lon2)
{
		

		const double R = 6371e3; // in meters
		const double phi_1 = lat1 * M_PI / 180;
		const double phi_2 = lat2 * M_PI / 180;
		const double delta_phi = abs(lat2 - lat1) * M_PI / 180;
		const double delta_lambda = abs(lon2 - lon1) * M_PI / 180;

		double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
			cos(phi_1) * cos(phi_2) *
			sin(delta_lambda / 2) * sin(delta_lambda / 2);
		double c = 2 * atan2(sqrt(a), sqrt(1 - a));
		double d = R * c;

		return d;
}
