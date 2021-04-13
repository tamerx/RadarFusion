#include "Radar.h"
#include "Message.h"


Radar::Radar()
{
}

Radar::Radar(Message & newMessage, int id_)
{
	id = id_;
	ip[0] = newMessage.radar.ip[0];
	ip[1] = newMessage.radar.ip[1];
	ip[2] = newMessage.radar.ip[2];
	ip[3] = newMessage.radar.ip[3];

	latitude = newMessage.radar.latitude;
	longitude = newMessage.radar.longitude;
	northYaw = newMessage.radar.northYaw;
	centerYaw = newMessage.radar.centerYaw;
	fov = newMessage.radar.halfwidth * 2;
	minRadii = newMessage.radar.minRadii;
	maxRadii = newMessage.radar.maxRadii;
	calculateYawStartEnd();
	calculateSectorPolygon();
}

void Radar::update(Message & newMessage)
{
	latitude = newMessage.radar.latitude;
	longitude = newMessage.radar.longitude;
	northYaw = newMessage.radar.northYaw;
	centerYaw = newMessage.radar.centerYaw;
	fov = newMessage.radar.halfwidth * 2;
	minRadii = newMessage.radar.minRadii;
	maxRadii = newMessage.radar.maxRadii;

	calculateYawStartEnd();
	calculateSectorPolygon();
}

void Radar::newTrackMessage(Message & newMessage)
{

	bool newTrackFlag = true;
	// check if this track id came before (any info came about this track before?)
	for (int i = 0; i < trackLists.size(); i++)
	{
		// if yes, and the trackstatus of the new info is DROPPED (0),
		// delete all instances and the trackList belongs to that trac
		
		if (trackLists[i].id == newMessage.trackInstance.id && newMessage.trackInstance.status == 0)
		{


			mtx->lock();
			for (std::vector<TrackMatchPair>::iterator it = trackPairs2.begin(); it != trackPairs2.end();)
			{
				std::string s = it->myuniqeid;
				std::vector<std::string> IDS = split(s, '_');
				std::string firstID = IDS.front();
				std::string secondID = IDS[1];

				if (std::to_string(newMessage.trackInstance.id) == firstID && std::to_string(newMessage.trackInstance.id) == secondID)
				{
					// to-do
					/* aþaðýdaki kod parçalarý ; Encode ile ilgili olan iþlemler */


					//MessageHeader tempMessageHeader;
					//Message tempMessage(tempMessageHeader);
					////it->setAsFused();
					//it->trackLast.dropOrDetected = 0;
					//tempMessage.trackInstance = it->trackLast;
					//int currentByIndex = 0;

					//tempMessage.encode(currentByIndex);
					//tempMessageHeader.encode(currentByIndex);

					////tempMessage.encode();
					//static const int allDataSize = currentByIndex;
					//unsigned char* allData = new unsigned char[allDataSize];
					//memcpy(allData, tempMessageHeader.headerData, 4);
					//memcpy(allData + 4, tempMessage.messageData, 60);

					//byteArrayList.push_back(allData);


					//Server server;
					//server.init(3);
					//server.waitForClient();

					//int message_length;
					//int recv_header_flag;
					//int recv_message_flag;

					//char * s1 = reinterpret_cast<char *>(allData);

					//recv_header_flag = server.sendNBytesFromSocket(server.ClientSocket, s1, MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES);
					//std::cout << "Sending data " << std::endl;


					it = trackPairs2.erase(it);

				}
				else
				{
					it++;
				}
			}

			mtx->unlock();

			mtx_radar->lock();
			trackLists.erase(trackLists.begin() + i);
			mtx_radar->unlock();
			newTrackFlag = false;
			break;
		}
		// if yes, and the trackstatus of the new info is NOT DROPPED (1)
		// add to end of the previous track infos,
		else if (trackLists[i].id == newMessage.trackInstance.id && newMessage.trackInstance.status == 1)
		{
			trackLists[i].newTrackInstance(newMessage);
			newTrackFlag = false;
			break;
		}
	}

	// if not create a new track list and new track instance then add it to the list of tracklists
	// but only do it when the new messages trackStatus is NOT DROPPED (1)
	if (newTrackFlag && newMessage.trackInstance.status == 1)
	{
		TrackList newTrackList(newMessage, id);
		newTrackList.newTrackInstance(newMessage);
		trackLists.push_back(newTrackList);
	}
}

bool Radar::isTrackInside(TrackInstance & track)
{
	return GisUtil::isPointInsidePolygon(Coordinate(track.latitude, track.longitude), sectorPolygon);
}

bool Radar::checkIfSameIpAddress(unsigned char *current,unsigned char *newIp)
{
	if (current[0] != newIp[0] || current[1] != newIp[1] || current[2] != newIp[2] || current[3] != newIp[3])
		return false;
	return true;
}

void Radar::calculateYawStartEnd()
{
	yawStart = (int)((centerYaw - (fov / 2)) + 360) % 360;
	yawEnd = (int)(centerYaw + (fov / 2)) % 360;
}

std::vector<Coordinate> Radar::calculateSectorPolygon()
{
	// 6 point corners method
		// sector points MUST be added in clockwise order
	sectorPolygon.clear();
	Coordinate temp;

	// Add close left and far left boundary points first
	temp = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), yawStart, minRadii);
	sectorPolygon.push_back(temp);


	Coordinate temp2;
	temp2 = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), yawStart, maxRadii);
	sectorPolygon.push_back(temp2);

	// Add far arc points in clockwise order
	temp = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), centerYaw, maxRadii);
	sectorPolygon.push_back(temp);


	// Add right up and right bottom boundary points
	temp = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), yawEnd, maxRadii);
	sectorPolygon.push_back(temp);

	temp = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), yawEnd, minRadii);
	sectorPolygon.push_back(temp);

	// Add close arc points in anti-clockwise order last
	temp = GisUtil::calculateP2FromP1BearingDistance(Coordinate(latitude, longitude), centerYaw, minRadii);
	sectorPolygon.push_back(temp);

	return sectorPolygon;

}

void Radar::radarFusion()
{

	try {

		
		// printf("Fusion started\n");

		// do not do anything if there is no radars inside
		if (radars.size() == 0)
			return;

		// Just before Radar Fusion to start;
		// Remove old tracks (older than 30 seconds(?)) from track lists
	/*	TrackList trackList;
		mtx_radar->lock();
		trackList.removeOldTracks(radars);
		mtx_radar->unlock();*/
	
		

		double DISTANCE_THRESHOLD = 100;       // meters
		double VELOCITY_THRESHOLD = 30;        // km/h ?
		double DIRECTION_ANGLE_THRESHOLD = 40; // degrees
		// TODO: maybe take them from a config file ?
		this->mtx_radar->lock();
		// fuse loop 1: for every radar in radarlist, except the last one
		for (int i = 0; i < radars.size() - 1; i++)
		{
			Radar &radar1 = radars[i];

			// fuse loop 2: compare radar1 tracks with every other radar tracks comes after radar1
			for (int j = i + 1; j < radars.size(); j++)
			{
				Radar &radar2 = radars[j];

				// reset the isFused variables
				for (auto trackList = radar1.trackLists.begin(); trackList != radar1.trackLists.end(); trackList++)
					for (auto track = trackList->tracks.begin(); track != trackList->tracks.end(); track++)
						track->isFused = false;

				for (auto trackList = radar2.trackLists.begin(); trackList != radar2.trackLists.end(); trackList++)
					for (auto track = trackList->tracks.begin(); track != trackList->tracks.end(); track++)
						track->isFused = false;

				// compare all the tracks (cross comparison)
				// TODO: if we get radar informations about GÖLBEK RADARS, maybe call Radar::isTrackInside() ?

				// this vector will hold the possible matches of every track pair that passed thresholds
				std::vector<TrackMatchPair> trackPairs;

				// loop for every track lists in the first radar
				for (auto trackList1 = radar1.trackLists.begin(); trackList1 != radar1.trackLists.end(); trackList1++)
				{

					// loop for every track list in the second radar
					for (auto trackList2 = radar2.trackLists.begin(); trackList2 != radar2.trackLists.end(); trackList2++)
					{
						if (radar1.isTrackInside(trackList2->tracks.back()) && radar2.isTrackInside(trackList1->tracks.back()))
						{
							// now compare just the last informations (last track instances) of the track lists
							// check for distance, velocity and direction angle thresholds
							// float distance_diff = lla2flatDistanceFormula(trackList1->tracks.end()->latitude,
							double distance_diff = GisUtil::lla2flatDistanceFormula(trackList1->tracks.back().latitude,
								trackList1->tracks.back().longitude,
								trackList2->tracks.back().latitude,
								trackList2->tracks.back().longitude);

							float velocity_diff = abs(trackList1->tracks.back().velocity - trackList2->tracks.back().velocity);
							float direction_angle_diff = abs(trackList1->tracks.back().directionAngle - trackList2->tracks.back().directionAngle);

							if (distance_diff <= DISTANCE_THRESHOLD)
							{ 
								TrackMatchPair p(&(trackList1->tracks[trackList1->tracks.size() - 1]), &(trackList2->tracks[trackList2->tracks.size() - 1]));
								std::cout << " Fuse oluþtu" << std::endl;
								mtx->lock();
								trackPairs.push_back(p);
								mtx->unlock();

							}
						}
					}
				}

				// when all the possible track matches added to the track match pair vector,
				// begin to match them, starting from the lowest cost to highest cost
				while (trackPairs.size() > 0)
				{
					for (std::vector<TrackMatchPair>::iterator it = trackPairs.begin();it != trackPairs.end();) 
					{
						if (it->track1ptr->isFused || it->track2ptr->isFused)
						{
							it = trackPairs.erase(it);
							if (trackPairs.size() <= 0)
								break;
						}
						else
						{
							++it;
						}
					}

					

					if (trackPairs.size() <= 0) {
						break;
					}

					// find the pair match that got the minimum cost if the tracks inside havent fused yet
					double minCost = trackPairs.begin()->cost;
					auto minCostTrackPair = trackPairs.begin();
					
					for (auto trackPair = trackPairs.begin(); trackPair != trackPairs.end(); trackPair++)
					{
						if (trackPair->cost <= minCost)
						{
							minCost = trackPair->cost;
							minCostTrackPair = trackPair;
						}
					}
					mtx->lock();
					this->trackPairs2.push_back(*minCostTrackPair);
					TrackMatchPair tempPair;
					
					for (TrackMatchPair& trackPair : this->trackPairs2)
					{

						if (minCostTrackPair->track1ptr->id == trackPair.track1ptr->id && minCostTrackPair->track2ptr->id == trackPair.track2ptr->id)
						{
							trackPair.trackMatchCount++;
							tempPair = trackPair;
						}
						else
						{
							if (tempPair.trackMatchCount >= Radar::countConstant)
							{


								Message trackPairToMessage;
								trackPairToMessage.trackInstance = tempPair.trackLast;
								trackPairToMessage.fuseFlag = true;
								this->fuseMessage = &trackPairToMessage;
								
								// sendFuseDataToMirsad fonksionu burda çaðrýlabilir


								//Message::appendFuseDataToCSV(tempPair.trackLast, tempPair.newUniqueID, tempPair.trackMatchCount, tempPair.myuniqeid);
								break;
							}
						}
					}


					mtx->unlock();
					minCostTrackPair->setAsFused();
					trackPairs.erase(minCostTrackPair);

				}
			}
		}
		this->mtx_radar->unlock();
		// printf("Fusion ended.\n");

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

void Radar::radarFusionThreadFunction(std::vector<Radar>& radars, int * radarsVectorIsBeingUsed)
{
	// call radar fusion in a infinite loop
	while (1)
	{
		if (*radarsVectorIsBeingUsed == 0)
		{

			// block anything to use radars vector
			*radarsVectorIsBeingUsed = 1;
			// printf("Blocked by Fusion 11\n");

			// change the radars vector
		   //radarFusion(radars);

			// remove the blocking
			*radarsVectorIsBeingUsed = 0;
			// printf("Block removed by Fusion 12\n");

			Sleep(100);
		}
	}
}

int Radar::getRadarVectorSize() const
{
	return radars.size();
}

std::vector<Radar> Radar::getRadars()
{
	return radars;
}

void Radar::setRadars(std::vector<Radar> radars)
{
	this->radars = radars;

}



std::vector<std::string> Radar::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

void Radar::debugArray(const unsigned char* data, size_t len) {
	std::ios_base::fmtflags f(std::cout.flags());
	for (size_t i = 0; i < len; ++i)
		std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (((int)data[i]) & 0xFF) << " ";
	std::cout << std::endl;
	std::cout.flags(f);
}