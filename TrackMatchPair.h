#ifndef TRACKMATCHPAIR
#define TRACKMATCHPAIR

#include "TrackInstance.h"
#include "Coordinate.h"
#include "ByteOperations.h"


class TrackMatchPair {
public:
	double cost;

	TrackInstance * track1ptr;
	TrackInstance * track2ptr;

	TrackInstance trackFirst;
	TrackInstance trackLast;

	int trackMatchCount = 0;
	std::string myuniqeid;
	long newUniqueID;
	

	TrackMatchPair();
	TrackMatchPair(TrackInstance * track1, TrackInstance * track2);


	void createNewUniqueID();


	// if this pair got the minimum cost,
	// set the isFused statuses of track1 and track2
	void setAsFused();


	// Takes two track instances (last in the any trackList.tracks)
	// calculates the similarity of the tracks; if they are very similar (possibly same), 
	// the cost is low; if they are less similar (not same), cost is high.
	// Returns the cost
	double calculateCost();

	// Takes a track instance and a time value as miliseconds,
	// calculates the location of the given track after given time value,
	// and returns the location as Coordinate object.
	Coordinate calculateTheCurrentLocation(TrackInstance &track, long deltaTime);
	


};


#endif