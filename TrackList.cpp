#include "TrackList.h"
#include "Message.h"

#define REMOVE_OLD_TRACKS_TIME_CONSTANT 10000 // seconds

TrackList::TrackList()
{
}

TrackList::TrackList(Message & newMessage, int radarId)
{
	id = newMessage.trackInstance.id;
	sourceRadarId = radarId;

	nInstanceCount = 0;
}

void TrackList::newTrackInstance(Message & newMessage)
{
	
	TrackInstance newTrackInstance(newMessage);
	tracks.push_back(newTrackInstance);
	nInstanceCount++;
}

float TrackList::getMIRSADTimeDifference(unsigned long long t1, unsigned long long t2)
{
	
	
	return (float)abs(long (t1 - t2)) ; // 1000 comes from miliseconds in values
}

void TrackList::removeOldTracks(std::vector<Radar>& radars)
{
	//printf("Removing old tracks ... ");

	//long currentTime = std::time(0);;


	// for each radar
	for (auto radar = radars.begin(); radar != radars.end(); radar++) {
		// for each track list (contains the information about a unique track (id) in different times)
		for (std::vector<TrackList>::iterator trackList = radar->trackLists.begin(); trackList != radar->trackLists.end();) {
			for (std::vector<TrackInstance>::iterator it = trackList->tracks.begin(); it != trackList->tracks.end();) {

				if (getMIRSADTimeDifference(radar->lastIncomingTrackTime, it->trackTime) > REMOVE_OLD_TRACKS_TIME_CONSTANT) {
					it = trackList->tracks.erase(it);
					//std::cout << "trackList->tracks.size() :: " << trackList->tracks.size() << std::endl;
					if (trackList->tracks.size() <= 0)
						break;
				}
				else
				{
					++it;
				}
			}

			if (trackList->tracks.size() <= 0) {

				// remove trackList from trackLists
				trackList = radar->trackLists.erase(trackList);
				if (radar->trackLists.size() <= 0)
					break;

				// set one pointer back
				//trackList--;
			}
			else
			{
				++trackList;
			}
		}
	}
}

	//printf("Done.\n");


// Takes references of radars
// and removes tracks that older than REMOVE_OLD_TRACKS_TIME_CONSTANT (defined in main for now)
void TrackList::removeOldPairs2(std::vector<TrackMatchPair> &trackPairs, std::vector<Radar> &radars)
{
	// 20 seconds for clearing trackparis2 list

	unsigned long long lastIncomingTrackTime_radar;
	if (radars.size()==1)
	{
		lastIncomingTrackTime_radar = radars[0].lastIncomingTrackTime;
	}

	else {


		if (radars[0].lastIncomingTrackTime >= radars[1].lastIncomingTrackTime)
		{
			lastIncomingTrackTime_radar = radars[0].lastIncomingTrackTime;
		}
		else
		{
			lastIncomingTrackTime_radar = radars[1].lastIncomingTrackTime;

		}

	}




	    std::cout << "Clear Size Track Pair size :: " << trackPairs.size() << std::endl;
		for (std::vector<TrackMatchPair>::iterator it = trackPairs.begin(); it != trackPairs.end();)
		{
			//if (getMIRSADTimeDifference(lastIncomingTrackTime_radar, it->trackFirst.trackTime) > 3000)
			if (getMIRSADTimeDifference(lastIncomingTrackTime_radar, it->trackFirst.trackTime) >5000 || getMIRSADTimeDifference(lastIncomingTrackTime_radar, it->trackLast.trackTime) > 5000)
			{

				/*std::cout << " Diffrence Time :: " << (float)abs(long(lastIncomingTrackTime_radar - it->trackFirst.trackTime)) << std::endl;
				std::cout << " Diffrence Time :: " << (float)abs(long(lastIncomingTrackTime_radar - it->trackLast.trackTime)) << std::endl;*/
				//trackPairs.clear();
				it=trackPairs.erase(it);
				
				
			}
			else
			{
				it++;
			}
		}

		//printf("Done.\n");
	
}

