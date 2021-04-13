#ifndef TRACKLIST
#define TRACKLIST


#include <vector>
#include "TrackInstance.h"
#include "TrackMatchPair.h"

class Radar;

class TrackList {
public:
	int id;
	int sourceRadarId;

	std::vector<TrackInstance> tracks;

	int nInstanceCount;

	//Radar radarPtr;

	TrackList();
	TrackList(Message &newMessage, int radarId);
	void newTrackInstance(Message &newMessage);
	float getMIRSADTimeDifference(unsigned long long t1, unsigned long long t2);
	void removeOldTracks(std::vector <Radar> &radars);
	void removeOldPairs2(std::vector<TrackMatchPair> &trackPairs, std::vector<Radar> &radars);


};


#endif