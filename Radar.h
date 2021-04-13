#ifndef RADAR
#define RADAR

#include <vector>
#include <iostream>
#include <mutex>
#include <iomanip>


#include "Coordinate.h"
#include "TrackList.h"
#include "TrackList.h"
#include "TrackMatchPair.h"
#include "GisUtil.h"

class Radar {
public:
	int id;
	unsigned char ip[4];
	double latitude;
	double longitude;
	double northYaw;
	double centerYaw;
	double fov;
	double yawStart;
	double yawEnd;
	double minRadii;
	double maxRadii;
	double halfwidth;

	static const int countConstant = 2;


	
	unsigned long long lastIncomingTrackTime;
	std::vector<Radar> radars;
	std::vector<Coordinate> sectorPolygon; // Coordinates of the radar's view corner points
	std::vector<TrackList> trackLists; // Will store the tracks that came from its radar ip
	std::vector<Radar> getRadars();
	std::vector<TrackMatchPair> trackPairs2;
	std::vector<unsigned char*> byteArrayList;
	std::mutex *mtx;
	std::mutex *mtx_radar;

	Message *fuseMessage;

	

	Radar();
	Radar(Message &newMessage, int id_);

	void update(Message &newMessage);


	void newTrackMessage(Message &newMessage);



	// checks if any track is inside of the radars view sector
	bool isTrackInside(TrackInstance &track);

	bool checkIfSameIpAddress(unsigned char currentIp[4],unsigned char newIp[4]);

	// calculates the angles of start of the sector and end of the sector
	// it needs to be called after every centerYaw or fov change
	void calculateYawStartEnd();

	// calculates the corner points of the polygon that represents the radars view sector
	std::vector<Coordinate> calculateSectorPolygon();
	void radarFusion();
	void radarFusionThreadFunction(std::vector<Radar> &radars, int *radarsVectorIsBeingUsed);
	int getRadarVectorSize() const;
	
	

	void setRadars(std::vector<Radar> radars );
	std::vector<std::string> split(const std::string &s, char delim);
	void debugArray(const unsigned char* data, size_t len);

};

#endif