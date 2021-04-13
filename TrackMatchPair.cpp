#include "TrackMatchPair.h"
#include "GisUtil.h"
TrackMatchPair::TrackMatchPair()
{

}
TrackMatchPair::TrackMatchPair(TrackInstance * track1, TrackInstance * track2)
{
	// hold the pointers to change isFused in them later on
	track1ptr = track1;
	track2ptr = track2;

	// check which one came last and assign them according to their time
	if (track1->trackTime < track2->trackTime) {
		trackFirst = *track1;
		trackLast = *track2;
	}
	else {
		trackFirst = *track2;
		trackLast = *track1;
	}

	createNewUniqueID();
	calculateCost();
	trackMatchCount++;
}

void TrackMatchPair::createNewUniqueID()
{
	int newId = track1ptr->id;
	for (int len_id2 = track2ptr->id; len_id2 > 0; len_id2 /= 10)
		newId *= 10;

	newUniqueID = newId + track2ptr->id;

	std::string firstID = std::to_string(track1ptr->id);
	std::string secondID = std::to_string(track2ptr->id);

	myuniqeid = firstID + "_" + secondID;
}

void TrackMatchPair::setAsFused()
{
	track1ptr->isFused = true;
	track2ptr->isFused = true;
}

double TrackMatchPair::calculateCost()
{
	// calculate the updated location coordinates of the one that came first
	Coordinate updatedLocation = calculateTheCurrentLocation(trackFirst, trackLast.trackTime - trackFirst.trackTime);
	double deltaDistance = GisUtil::lla2flatDistanceFormula(trackLast.latitude,
		trackLast.longitude,
		updatedLocation.lat,
		updatedLocation.lon);

	double deltaVelocity = trackLast.velocity - trackFirst.velocity;
	double deltaDirectionAngle = trackLast.directionAngle - trackFirst.directionAngle;

	if (deltaVelocity < 0)
		deltaVelocity *= -1;
	if (deltaDirectionAngle < 0)
		deltaDirectionAngle *= -1;

	cost = (5 * deltaDistance + 3 * deltaVelocity + 2 * deltaDirectionAngle) / 100;

	return cost;
}

Coordinate TrackMatchPair::calculateTheCurrentLocation(TrackInstance & track, long deltaTime)
{
	double deltaDistance = track.velocity * (deltaTime / 3600);
	// Coordinate oldCoordinate(track.latitude, track.longitude);

	return GisUtil::calculateP2FromP1BearingDistance(Coordinate(track.latitude, track.longitude),
		// oldCoordinate,
		track.directionAngle,
		deltaDistance);
}

