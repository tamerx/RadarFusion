
#include "Message.h"
extern HANDLE hSemaphore1;
extern HANDLE hSemaphore2;

Message::Message()
{


}

Message::Message(Radar r)
{

	this->radar = r;
	cursor = 0;
	std::mutex g_mutex;
}

Message::Message(MessageHeader messageHeader_)
{
	messageHeader = messageHeader_;
}

MessageHeader Message::getMessageHeader()
{
	return messageHeader;
}

void Message::setMessageHeader(MessageHeader messageHeader_)
{
	messageHeader = messageHeader_;
}

void Message::encode(int &currentByteIndex)
{
	ByteOperations byteOperations;
	std::string printToConsole = "printToConsole :: ";

	byteOperations.IntToByte(messageData, currentByteIndex, this->trackInstance.id);
	currentByteIndex += sizeof(INT);
	printToConsole += std::to_string(this->trackInstance.id) + " :: ";
	byteOperations.IntToByte(messageData, currentByteIndex, this->trackInstance.range);
	currentByteIndex += sizeof(INT);
	printToConsole += std::to_string(this->trackInstance.range) + " :: ";
	byteOperations.DoubleToByte(messageData, currentByteIndex, this->trackInstance.azimuth);
	currentByteIndex += sizeof(DOUBLE);
	printToConsole += std::to_string(this->trackInstance.azimuth) + " :: ";
	byteOperations.DoubleToByte(messageData, currentByteIndex, this->trackInstance.directionAngle);
	currentByteIndex += sizeof(DOUBLE);
	printToConsole += std::to_string(this->trackInstance.directionAngle) + " :: ";
	byteOperations.DoubleToByte(messageData, currentByteIndex, this->trackInstance.velocity);
	currentByteIndex += sizeof(DOUBLE);
	printToConsole += std::to_string(this->trackInstance.status) + " :: ";
	byteOperations.IntToByte(messageData, currentByteIndex, this->trackInstance.classificationStatus);
	currentByteIndex += sizeof(INT);
	printToConsole += std::to_string(this->trackInstance.classificationStatus) + " :: ";
	byteOperations.DoubleToByte(messageData, currentByteIndex, this->trackInstance.latitude);
	currentByteIndex += sizeof(DOUBLE);
	printToConsole += std::to_string(this->trackInstance.latitude) + " :: ";
	byteOperations.DoubleToByte(messageData, currentByteIndex, this->trackInstance.longitude);
	currentByteIndex += sizeof(DOUBLE);
	printToConsole += std::to_string(this->trackInstance.longitude) + " :: ";

	this->messageHeader.setMessageLength(currentByteIndex);
	//std::cout << printToConsole << std::endl;
}

void Message::decode(unsigned char *data)
{

	ByteOperations byteOperations;
	int currentByteIndex = MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES;

	// Track ID
	this->trackInstance.id = (int)byteOperations.unsignedByteToInt(data, currentByteIndex);
	currentByteIndex += sizeof(INT);

	// Track Range
	this->trackInstance.range = (int)byteOperations.unsignedByteToInt(data, currentByteIndex);
	currentByteIndex += sizeof(INT);

	// Track Azimuth
	memcpy(&this->trackInstance.azimuth, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Velocity
	memcpy(&this->trackInstance.velocity, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Direction Angle
	memcpy(&this->trackInstance.directionAngle, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Radial Velocity
	memcpy(&this->trackInstance.radialVelocity, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Status
	this->trackInstance.status = (int)byteOperations.unsignedByteToInt(data, currentByteIndex);
	currentByteIndex += sizeof(INT);

	// Track Classification Status
	this->trackInstance.classificationStatus = (int)byteOperations.unsignedByteToInt(data, currentByteIndex);
	currentByteIndex += sizeof(INT);

	// Track Snr
	memcpy(&this->trackInstance.snr, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Ip
	this->radar.ip[0] = data[currentByteIndex];
	currentByteIndex += sizeof(CHAR);
	this->radar.ip[1] = data[currentByteIndex];
	currentByteIndex += sizeof(CHAR);
	this->radar.ip[2] = data[currentByteIndex];
	currentByteIndex += sizeof(CHAR);
	this->radar.ip[3] = data[currentByteIndex];
	currentByteIndex += sizeof(CHAR);

	// Radar Latitude
	memcpy(&this->radar.latitude, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Longitude
	memcpy(&this->radar.longitude, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar North Yaw
	memcpy(&this->radar.northYaw, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Center Angle
	memcpy(&this->radar.centerYaw, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Halfwidth
	memcpy(&this->radar.halfwidth, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Min Radii
	memcpy(&this->radar.minRadii, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Radar Max Radii
	memcpy(&this->radar.maxRadii, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Latitude
	memcpy(&this->trackInstance.latitude, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Longitude
	memcpy(&this->trackInstance.longitude, &data[currentByteIndex], sizeof(DOUBLE));
	currentByteIndex += sizeof(DOUBLE);

	// Track Time
	this->trackInstance.trackTime = byteOperations.unsignedByteToLong(data, currentByteIndex);
	currentByteIndex += sizeof(LONG);

	
	// Get and set local time
	//localTime = std::time(0);
}

int Message::computeMessageLength()
{
	return messageHeader.getMessageLength();
}

void Message::setTrackParameters(int id, int range, double azimuth, double velocity, double directionAngle, double radialVelocity, int status, int classificationStatus, double snr, double latitude, double longitude, long time)
{

	trackInstance.id = id;
	trackInstance.range = range;
	trackInstance.azimuth = azimuth;
	trackInstance.velocity = velocity;
	trackInstance.directionAngle = directionAngle;
	trackInstance.radialVelocity = radialVelocity;
	trackInstance.status = status;
	trackInstance.classificationStatus = classificationStatus;
	trackInstance.snr = snr;
	trackInstance.latitude = latitude;
	trackInstance.longitude = longitude;
	trackInstance.trackTime = time;
}

void Message::setRadarParameters(unsigned char ip[4], int radar_id, double latitude, double longitude, double northYaw, double centerAngle, double halfwidth, double minRadii, double maxRadii)
{
	radar.ip[0] = ip[0];
	radar.ip[1] = ip[1];
	radar.ip[2] = ip[2];
	radar.ip[3] = ip[3];
	radar.id = radar_id;
	radar.latitude = latitude;
	radar.longitude = longitude;
	radar.northYaw = northYaw;

	radar.centerYaw = centerAngle;
	radar.halfwidth = halfwidth;
	radar.minRadii = minRadii;
	radar.maxRadii = maxRadii;
}

void Message::processNewMessage(Message &newMessage)
{
	//printf("Processing new message...\n");

	// hld radar id for real time track display
	int radarId;

	bool newRadarFlag = true;

	// check if the source radar id/ip is seen/saved before
	for (int i = 0; i < radar.getRadarVectorSize(); i++)
	{
		// if yes, first update radar parameters and then send new track/message info into it

		if (radar.radars[i].checkIfSameIpAddress(radar.radars[i].ip,newMessage.radar.ip))
		{
			
			radarId = radar.radars[i].id;
			if (radarId == 1)
			{
				newMessage.trackInstance.trackRadarID = 1;
			}
			else if (radarId == 2)
			{
				newMessage.trackInstance.trackRadarID = 2;
			}
			radar.radars[i].update(newMessage);
			radar.radars[i].newTrackMessage(newMessage);
			newRadarFlag = false;

			break;
		}
	}
	if (newRadarFlag)
	{
		// if not, create a new radar object
		Radar newRadar(newMessage, radar.getRadarVectorSize() + 1);
		// add track to it now
		
		// add it to the radar list
		radar.radars.push_back(newRadar);
		radarId = radar.radars.back().id;
		if (radarId == 1)
		{
			newMessage.trackInstance.trackRadarID = 1;
		}
		else if (radarId == 2)
		{
			newMessage.trackInstance.trackRadarID = 2;
		}


		newRadar.newTrackMessage(newMessage);
		
	}
	
	

	//appendRawDataToCSV(newMessage, radarId);

	//// update last incoming track time in all radars
	for (auto r = radar.radars.begin(); r != radar.radars.end(); r++)
	{
		r->lastIncomingTrackTime = newMessage.trackInstance.trackTime;
	}

	//// Call Radar Fusion HERE ???

		radar.radarFusion();



	

	//std::cout << "End of new message processing.\n" << std::endl;
}

void Message::processMessageFromFile()
{

	std::thread processMessageFromFile_THREAD = proces_message_thread();
	std::thread processFusedMessageFromFile_THREAD = proces_fuse_message_thread();
	

	processMessageFromFile_THREAD.join();
	processFusedMessageFromFile_THREAD.join();
	
	
}


void Message::processMessageFromFileThreadFunction()
{
	while (true)
	{
		WaitForSingleObject(hSemaphore2, INFINITE);

		std::cout << "--------------- reading from file" << std::endl;

		//++num_producers_working;
		readData(cursor, messages);
		processNewMessage(messages[cursor]);
		

		if (this->fuseFlag==true)
		{
			Message trackPairToMessage = *this->radar.fuseMessage;
			messages.push_back(trackPairToMessage);
			cursor += 1;

		}
		

		//is_not_empty->notify_all();
		//std::this_thread::sleep_for(std::chrono::milliseconds(producer_delay_to_produce));
		//--num_producers_working;

		ReleaseSemaphore(hSemaphore1, 1, 0);
	}

}

void Message::processFusedMessageFromFileThreadFunction()
{
	Server server_gonder_1;
	server_gonder_1.init(3);

	Server server_gonder_2;
	server_gonder_2.init(4);


	//server_gonder_1.waitForClient();
	//server_gonder_2.waitForClient();


	while (true)
	{
		WaitForSingleObject(hSemaphore1, INFINITE);

		std::cout << "--------------- sending message" << std::endl;
	


		//while (num_producers_working != 0 ) {
		

				sendFuseDataToMirsad(cursor, server_gonder_1, server_gonder_2);
				cursor++;
				std::cout << "curser:" << cursor << std::endl;
			
				ReleaseSemaphore(hSemaphore2, 1, 0);
		
			//std::this_thread::sleep_for(std::chrono::milliseconds(consumer_delay_to_consume));
		//}	
	}

	server_gonder_1.terminate();
	server_gonder_2.terminate();
}

// Takes which radar data file to read (1 or 2), number of messages to be read, and messages list
// reads given n messages
// if it is end of the file before n, reads until the end of the file
// and returns the number of messages read
void Message::readData(int cursor,  std::vector<Message> &messages)
{

	//std::cout << " normal data --> " << startFromNthLine << std::endl;
	std::string file_path;
	int nLinesRead = 0;
	file_path = "data/radar_fused2.csv";

	// Create an input filestream
	std::ifstream myFile(file_path);
	//FILE *fptr;

	int id;
	int range;
	double azimuth;
	double velocity;
	double directionAngle;
	double radialVelocity;
	int status;
	int classificationStatus;
	double snr;

	double latitude;
	double longitude;
	long time;

	long localTime;

	int radar_id;

	char tempS[200];

	// Open an existing file
	// Make sure the file is open
	if (!myFile.is_open())
		throw std::runtime_error("Could not open file");

	// Helper var
	std::string line, word;
	std::vector<std::string> lineWords;

	// Read the column names
	if (myFile.good())
	{
		// Extract the first line in the file
		std::getline(myFile, line);
		//std::cout <<"Line: " <<line <<std::endl;

		// Create a stringstream from line
		std::stringstream ss(line);

		// Extract each column name
		while (std::getline(ss, word, ';'))
		{
			
			lineWords.push_back(word);
		}
	}

	while (std::getline(myFile, line))
	{
		

	
		// Create a stringstream of the current line
		std::stringstream ss(line);

		if (nLinesRead < cursor)
		{
			nLinesRead++;
			continue;
		}


		lineWords.clear();
		// Extract each column name
		while (std::getline(ss, word, ';'))
			lineWords.push_back(word);



		id = std::stoi(lineWords[0]);
		range = std::stoi(lineWords[1]);
		azimuth = std::stod(lineWords[2]);

		velocity = std::stod(lineWords[3]);
		directionAngle = std::stod(lineWords[4]);
		radialVelocity = std::stod(lineWords[5]);
		status = std::stoi(lineWords[6]);
		classificationStatus = std::stoi(lineWords[7]);
		snr = std::stod(lineWords[8]);

		latitude = std::stod(lineWords[9]);
		longitude = std::stod(lineWords[10]);

		// gives runtime error when we try to convert full time number (string) to long
		char *timeStr = strdup(lineWords[12].c_str());
		time = std::atoll(timeStr);
		//time = std::stol(lineWords[12]);
		//time=0;

		radar_id = std::stoi(lineWords[13]);
		int trackRadarID = 0;
		
		

		// check if read latitude and longitude is zero, if yes ignore that line
		if (latitude == 0 || longitude == 0)
		{
			return;
		}

		Message newMessage;

		if (radar_id == 1)
		{
			unsigned char ip_[4] = { 50, 31, 29, 132 };
			trackRadarID = 1;
			newMessage.setRadarParameters(ip_, radar_id, std::stod(lineWords[14]), std::stod(lineWords[15]), std::stod(lineWords[16]), std::stod(lineWords[17]), std::stod(lineWords[18]), std::stod(lineWords[19]), std::stod(lineWords[20]));
		}
		else if (radar_id == 2)
		{
			unsigned char ip_[4] = { 50, 31, 29, 133 };
			trackRadarID = 2;
			newMessage.setRadarParameters(ip_, radar_id, std::stod(lineWords[14]), std::stod(lineWords[15]), std::stod(lineWords[16]), std::stod(lineWords[17]), std::stod(lineWords[18]), std::stod(lineWords[19]), std::stod(lineWords[20]));
		}

		if (radar_id == 1 || radar_id == 2)
		{
			newMessage.setTrackParameters(id, range, azimuth, velocity, directionAngle, radialVelocity, status, classificationStatus, snr, latitude, longitude, time);
			newMessage.trackInstance.trackRadarID = trackRadarID;
			messages.push_back(newMessage);
		}

		break;
		}


	// Close file
	myFile.close();

}


void Message::sendFuseDataToMirsad(int cursor, Server &server_gonder_1, Server &server_gonder_2)
{


	if (this->radar.fuseMessage->fuseFlag==true)
	{
		// füzyon olduðu durum
	}


	Message* message = &messages[cursor];
	TrackInstance trackInstanceForFileData;

	static int number_of_times = 0;

	trackInstanceForFileData.id = message->trackInstance.id;
	trackInstanceForFileData.range = message->trackInstance.range;
	trackInstanceForFileData.azimuth = message->trackInstance.azimuth;

	trackInstanceForFileData.velocity = message->trackInstance.velocity;
	trackInstanceForFileData.directionAngle = message->trackInstance.directionAngle;
	trackInstanceForFileData.radialVelocity = message->trackInstance.radialVelocity;
	trackInstanceForFileData.status = message->trackInstance.status;
	trackInstanceForFileData.classificationStatus = message->trackInstance.classificationStatus;
	trackInstanceForFileData.snr = message->trackInstance.snr;

	trackInstanceForFileData.latitude = message->trackInstance.latitude;
	trackInstanceForFileData.longitude = message->trackInstance.longitude;

	// gives runtime error when we try to convert full time number (string) to long
	trackInstanceForFileData.time = message->trackInstance.time;
	//time = std::stol(lineWords[12]);
	//time=0;

	int radar_id = message->radar.id;

	std::string myUnuqiueID = std::to_string(message->trackInstance.trackRadarID);

	


	int trackRadarID = message->trackInstance.trackRadarID;

	// check if read latitude and longitude is zero, if yes ignore that line
	if (trackInstanceForFileData.latitude == 0 || trackInstanceForFileData.longitude == 0)
	{

		return;

	}

	if (this->radar.trackPairs2.size() == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}


	//std::this_thread::sleep_for(std::chrono::milliseconds(200));
	this->radar.mtx->lock();

	bool isFusedDataInTrackPairs = false;
	bool isFusedDataInFusedArray = false;

	std::string firstID = "";
	std::string secondID = "";



	//std::cout << "Sending data :: " << ++number_of_times << std::endl;

	if (radar_id == 12) {



		std::vector<std::string> IDS = split(myUnuqiueID, '_');
		firstID = IDS.front();
		secondID = IDS[1];

		trackInstanceForFileData.id = std::stoi(firstID);

		for (std::vector<TrackMatchPair>::iterator trackPairsiterator = this->radar.trackPairs2.begin(); trackPairsiterator != this->radar.trackPairs2.end(); trackPairsiterator++)
		{

			if (trackPairsiterator->track1ptr->id == std::stoi(firstID) && trackPairsiterator->track2ptr->id == std::stoi(secondID))
			{

				isFusedDataInTrackPairs = true;
				break;
			}
		}


		// Create an iterator of map
		std::map<std::string, int>::iterator it;
		// Find the element with key 'hat'
		it = fusedArray.find(myUnuqiueID);
		// Check if element exists in map or not
		if (it != fusedArray.end())
		{
			isFusedDataInFusedArray = true;
		}


		if (isFusedDataInFusedArray == false && isFusedDataInTrackPairs == true)
		{

			std::map<std::string, int>::iterator it = fusedArray.begin();
			fusedArray.insert(it, std::pair<std::string, int>(myUnuqiueID, trackRadarID));

		}

		else if (isFusedDataInFusedArray == true && isFusedDataInTrackPairs == false)
		{

			fusedArray.erase(myUnuqiueID);

			//fusedArray.erase(std::remove(fusedArray.begin(), fusedArray.end(), myUnuqiueID), fusedArray.end());
		}

	}




	std::map<std::string, int>::iterator t2;
	for (t2 = fusedArray.begin(); t2 != fusedArray.end(); )
	{

		bool isinside = false;
		TrackMatchPair tempPair;

		std::vector<std::string> IDS = split(t2->first, '_');
		std::string firstID_it = IDS.front();
		std::string  secondID_it = IDS[1];

		for (std::vector<TrackMatchPair>::iterator it3 = this->radar.trackPairs2.begin(); it3 != this->radar.trackPairs2.end(); it3++)
		{

			tempPair = *it3;
			if (it3->track1ptr->id == std::stoi(firstID_it) && it3->track2ptr->id == std::stoi(secondID_it))
			{
				isinside = true;
				break;
			}


		}

		if (isinside == true)
		{

			TrackInstance deletedTrackInstance;
			int id = 0;
			std::vector<std::string> IDS = split(t2->first, '_');
			std::string firstID_it_true = IDS.front();
			std::string  secondID_it_true = IDS[1];

			if (t2->second == 1)
			{
				id = std::stoi(firstID_it_true);
			}
			else if (t2->second == 2)
			{
				id = std::stoi(secondID_it_true);
			}

			deletedTrackInstance.id = id;
			deletedTrackInstance.trackRadarID = t2->second;

			MessageHeader tempMessageHeader;
			Message tempMessage(tempMessageHeader);
			tempMessage.trackInstance = deletedTrackInstance;
			int currentByIndex = 0;
			tempMessage.encode(currentByIndex);

			tempMessageHeader.encode(currentByIndex, 1);
			static const int allDataSize = currentByIndex;
			unsigned char *allData = new unsigned char[allDataSize];
			memcpy(allData, tempMessageHeader.headerData, messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
			memcpy(allData + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES, tempMessage.messageData, currentByIndex);

			int message_length;
			int recv_header_flag;
			int recv_message_flag;
			char *s1 = reinterpret_cast<char *>(allData);
			//debugArray(allData, currentByIndex);
			if (tempMessage.trackInstance.trackRadarID == 1)
			{
				//recv_header_flag = server_gonder_1.sendNBytesFromSocket(server_gonder_1.ClientSocket, s1, currentByIndex + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
				appendRawDataToCSV_mirsad(tempMessage, deletedTrackInstance.trackRadarID, 1);
			}
			else if (tempMessage.trackInstance.trackRadarID == 2)
			{
				//recv_header_flag = server_gonder_2.sendNBytesFromSocket(server_gonder_2.ClientSocket, s1, currentByIndex + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
				appendRawDataToCSV_mirsad(tempMessage, deletedTrackInstance.trackRadarID, 1);
			}

			++t2;
		}
		else if (isinside == false)
		{





			TrackInstance deletedTrackInstance;
			int id = 0;
			std::vector<std::string> IDS = split(t2->first, '_');
			std::string firstID_it_true = IDS.front();
			std::string  secondID_it_true = IDS[1];

			if (t2->second == 1)
			{
				id = std::stoi(firstID_it_true);
			}
			else if (t2->second == 2)
			{
				id = std::stoi(secondID_it_true);
			}

			deletedTrackInstance.id = id;
			deletedTrackInstance.trackRadarID = t2->second;

			MessageHeader tempMessageHeader;
			Message tempMessage(tempMessageHeader);
			tempMessage.trackInstance = deletedTrackInstance;
			int currentByIndex = 0;
			tempMessage.encode(currentByIndex);

			tempMessageHeader.encode(currentByIndex, 0);
			static const int allDataSize = currentByIndex;
			unsigned char *allData = new unsigned char[allDataSize];
			memcpy(allData, tempMessageHeader.headerData, messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
			memcpy(allData + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES, tempMessage.messageData, currentByIndex);

			int message_length;
			int recv_header_flag;
			int recv_message_flag;
			char *s1 = reinterpret_cast<char *>(allData);
			debugArray(allData, currentByIndex);
			if (deletedTrackInstance.trackRadarID == 1)
			{
				//recv_header_flag = server_gonder_1.sendNBytesFromSocket(server_gonder_1.ClientSocket, s1, currentByIndex + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
				appendRawDataToCSV_mirsad(tempMessage, deletedTrackInstance.trackRadarID, 0);
			}
			else if (deletedTrackInstance.trackRadarID == 2)
			{
				//recv_header_flag = server_gonder_2.sendNBytesFromSocket(server_gonder_2.ClientSocket, s1, currentByIndex + messageHeader.MESSAGE_HEADER_SIZE_IN_BYTES);
				appendRawDataToCSV_mirsad(tempMessage, deletedTrackInstance.trackRadarID, 0);
			}

			t2 = fusedArray.erase(t2);

		}


	}



	TrackList trackList;
	this->radar.mtx_radar->lock();
	trackList.removeOldTracks(this->radar.radars);
	this->radar.mtx_radar->unlock();
	trackList.removeOldPairs2(this->radar.trackPairs2, this->radar.radars);
	this->radar.mtx->unlock();

}


void Message::printEverything(std::vector<Radar> radars)
{
	printf("Radar Count = %d\n", radars.size());
	for (auto radar = radars.begin(); radar != radars.end(); radar++)
	{
		printf("\tRadar  ID:%d  -  IP:%d.%d.%d.%d  -  Track List Count:%d\n", radar->id, radar->ip[0],
			radar->ip[1], radar->ip[2], radar->ip[3], radar->trackLists.size());
		for (auto trackList = radar->trackLists.begin(); trackList != radar->trackLists.end(); trackList++)
		{
			printf("\t\tTrack List  ID:%d  -  Tracks Count:%d\n", trackList->id, trackList->nInstanceCount);
			for (auto track = trackList->tracks.begin(); track != trackList->tracks.end(); track++)
				printf("\t\t\tTrack  Range:%d  -  Lat:%f  -  Lon:%f  -  Time:%ld\n", track->range,
					track->latitude, track->longitude, track->time);
		}
		printf("\n");
	}
}

// creates or resets the file that outputs of fusion will be saved
void Message::initializeOutputCSV()
{
	
		std::string file_path = "data/radar_fused.csv";
		std::ofstream ofs;
		ofs.open(file_path, std::ofstream::out);
		ofs << "ID;Range;Azimuth;Velocity;DirectionAngle;RadialVelocity;TrackStatus;ClassificationStatus;SNR;Latitude;Longitude;CreationStatus;UpdateTime;";
		ofs << "RadarId;RadarLatitude;RadarLongitude;RadarNorthYaw;RadarCenterAngle;RadarHalfwidth;RadarMinRadii;RadarMaxRadii;Count;MyUniqueID;TrackRadarID\n";
		ofs.close();
	
}

void Message::initializeOutputCSV_mirsad()
{
	std::string file_path = "data/radar_fused_mirsad.csv";

	std::ofstream ofs;
	ofs.open(file_path, std::ofstream::out);

	ofs << "ID;Range;Azimuth;Velocity;DirectionAngle;RadialVelocity;TrackStatus;ClassificationStatus;SNR;Latitude;Longitude;CreationStatus;UpdateTime;";
	ofs << "RadarId;RadarLatitude;RadarLongitude;RadarNorthYaw;RadarCenterAngle;RadarHalfwidth;RadarMinRadii;RadarMaxRadii;Count;MyUniqueID;TrackRadarID\n";

	ofs.close();
}

void Message::appendRawDataToCSV(Message &message, int radarId)
{
	std::string file_path = "data/radar_fused.csv";

	std::ofstream ofs;
	ofs.open(file_path, std::ofstream::out | std::ofstream::app);

	//ofs <<"ID;Range;Azimuth;Velocity;DirectionAngle;RadialVelocity;TrackStatus;ClassificationStatus;SNR;Latitude;Longitude;CreationStatus;UpdateTime;";
	//ofs <<"RadarId;RadarLatitude;RadarLongitude;RadarNorthYaw;RadarCenterAngle;RadarHalfwidth;RadarMinRadii;RadarMaxRadii\n";
	ofs << message.trackInstance.id << ";";
	ofs << message.trackInstance.range << ";";
	ofs << message.trackInstance.azimuth << ";";
	ofs << message.trackInstance.velocity << ";";
	ofs << message.trackInstance.directionAngle << ";";
	ofs << message.trackInstance.velocity << ";";
	ofs << message.trackInstance.status << ";";
	ofs << message.trackInstance.classificationStatus << ";";
	ofs << message.trackInstance.snr << ";";
	ofs << message.trackInstance.latitude << ";";
	ofs << message.trackInstance.longitude << ";";
	ofs << "0.0"
		<< ";";
	ofs << message.trackInstance.trackTime << ";";
	ofs << radarId << ";";

	//ofs <<message.radarIp <<";";
	ofs << message.radar.latitude << ";";
	ofs << message.radar.longitude << ";";
	ofs << message.radar.northYaw << ";";
	ofs << message.radar.centerYaw << ";";
	ofs << message.radar.halfwidth << ";";
	ofs << message.radar.minRadii << ";";
	ofs << message.radar.maxRadii << ";";
	ofs << 0 << ";";
	ofs << 0 << ";";
	ofs << message.trackInstance.trackRadarID << "\n";

	ofs.close();
}



void Message::appendRawDataToCSV_mirsad(Message &message, int radarId,int basladi_bitti)
{
	std::string file_path = "data/radar_fused_mirsad.csv";

	std::ofstream ofs;
	ofs.open(file_path, std::ofstream::out | std::ofstream::app);

	std::string basladi_bitti_flag = "";

	if (basladi_bitti==0)
	{
		basladi_bitti_flag = "bitti";
	}
	else if (basladi_bitti==1) {
		basladi_bitti_flag = "basladi";

	}

	//ofs <<"ID;Range;Azimuth;Velocity;DirectionAngle;RadialVelocity;TrackStatus;ClassificationStatus;SNR;Latitude;Longitude;CreationStatus;UpdateTime;";
	//ofs <<"RadarId;RadarLatitude;RadarLongitude;RadarNorthYaw;RadarCenterAngle;RadarHalfwidth;RadarMinRadii;RadarMaxRadii\n";
	ofs << message.trackInstance.id << ";";
	//ofs << message.trackInstance.range << ";";
	//ofs << message.trackInstance.azimuth << ";";
	//ofs << message.trackInstance.velocity << ";";
	//ofs << message.trackInstance.directionAngle << ";";
	//ofs << message.trackInstance.velocity << ";";
	//ofs << message.trackInstance.status << ";";
	//ofs << message.trackInstance.classificationStatus << ";";
	//ofs << message.trackInstance.snr << ";";
	//ofs << message.trackInstance.latitude << ";";
	//ofs << message.trackInstance.longitude << ";";
	//ofs << "0.0"
	//	<< ";";
	//ofs << message.trackInstance.trackTime << ";";
	//ofs << radarId << ";";

	////ofs <<message.radarIp <<";";
	//ofs << message.radar.latitude << ";";
	//ofs << message.radar.longitude << ";";
	//ofs << message.radar.northYaw << ";";
	//ofs << message.radar.centerYaw << ";";
	//ofs << message.radar.halfwidth << ";";
	//ofs << message.radar.minRadii << ";";
	//ofs << message.radar.maxRadii << ";";
	//ofs << 0 << ";";
	ofs << basladi_bitti_flag << ";";
	ofs << message.trackInstance.trackRadarID << "\n";


	ofs.close();
}

void Message::appendFuseDataToCSV(TrackInstance &track, long newId, int trackMatchCount, std::string myUniqueID)
{
	std::string file_path = "data/radar_fused.csv";

	std::ofstream ofs;
	ofs.open(file_path, std::ofstream::out | std::ofstream::app);

	//ofs <<"ID;Range;Azimuth;Velocity;DirectionAngle;RadialVelocity;TrackStatus;ClassificationStatus;SNR;Latitude;Longitude;CreationStatus;UpdateTime;";
	//ofs <<"RadarId;RadarLatitude;RadarLongitude;RadarNorthYaw;RadarCenterAngle;RadarHalfwidth;RadarMinRadii;RadarMaxRadii\n";

	// NEW ID = {FirstTrackId}{SecondTrackId} => if first track id = 584, and second = 239; new id = 584239

	ofs << newId << ";";
	ofs << track.range << ";";
	ofs << track.azimuth << ";";
	ofs << track.velocity << ";";
	ofs << track.directionAngle << ";";
	ofs << track.radialVelocity << ";";
	ofs << track.status << ";";
	ofs << track.classificationStatus << ";";
	ofs << track.snr << ";";
	ofs << track.latitude << ";";
	ofs << track.longitude << ";";
	ofs << "0"
		<< ";";
	ofs << track.trackTime << ";";
	ofs << 12 << ";";

	ofs << 0 << ";";
	ofs << 0 << ";";
	ofs << 0 << ";";
	ofs << 0 << ";";
	ofs << 0 << ";";
	ofs << 0 << ";";

	ofs << 0 << ";";

	ofs << trackMatchCount << ";";
	ofs << myUniqueID << ";";
	ofs << track.trackRadarID << "\n";
	ofs.close();
}

void Message::processMessageFromMirsad()
{

	int number = 1;
	int number2 = 2;

	std::thread server1 = server_thread(&number);
	std::thread server2 = server_thread(&number2);
	//std::thread processFusedMessageFromFile_THREAD = proces_fuse_message_thread();

	server1.join();
	server2.join();
	//processFusedMessageFromFile_THREAD.join();
}

int Message::server_thread_function(int serverParameter)
{
	// Loop for repeated connections
	while (true)
	{
		printf("Waiting for client to connect . . .\n");

		// initialize server and wait for the client to connect
		Server server;
		server.init((int)serverParameter);
		server.waitForClient();

		unsigned char recv_data[MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES + MAX_MESSAGE_LENGTH];
		int message_length;
		int recv_header_flag;
		int recv_message_flag;

		// Receive until the peer shuts down the connection
		while (true)
		{

			//MessageHeader messageHeader;
			// First read the message header
			recv_header_flag = server.readNBytesFromSocket(server.ClientSocket, recv_data, MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES);

			if (recv_header_flag == 1)
			{

				MessageHeader recvMessageHeader;
				recvMessageHeader.decode(recv_data);
				Message newMessage(recvMessageHeader);
				//printf("\nCurrent Time: %ld\n", std::time(0));
				//printf("\tNew Message - Length: %d\n", newMessage.messageHeader.getMessageLength());

				// Then read the message depending on its length
				recv_message_flag = server.readNBytesFromSocket(server.ClientSocket, &recv_data[MessageHeader::MESSAGE_HEADER_SIZE_IN_BYTES], newMessage.messageHeader.getMessageLength());

				if (recv_message_flag == 1)
				{

					try
					{
						// decode the message
						newMessage.decode(recv_data);
						// Add message to the messagelist
						this->messages.push_back(newMessage);


						//	if (radarsVectorIsBeingUsed == 0)
						//	{

						//		// block anything to use the radars vector
						//		radarsVectorIsBeingUsed = 1;

								processNewMessage(newMessage);
								//std::this_thread::sleep_for(std::chrono::milliseconds(11));
								//break;

					}
					catch (...)
					{
						printf("ERROR while decoding the received message\n");
					}
				}
				else
				{
					printf("Unexpected end of stream\n");
					return 1;
				}
			}
			else
			{
				printf("Connection closed.\n");
				break;
			}
		}
		server.terminate();
	}
}

std::thread Message::server_thread(int *serverParameter)
{
	return std::thread([=] { server_thread_function(serverParameter[0]); });
}

std::thread Message::proces_fuse_message_thread()
{
	return std::thread([=] { processFusedMessageFromFileThreadFunction(); });
}

std::thread Message::proces_message_thread()
{
	return std::thread([=] { processMessageFromFileThreadFunction(); });
}

void Message::debugArray(const unsigned char *data, size_t len)
{
	std::ios_base::fmtflags f(std::cout.flags());
	for (size_t i = 0; i < len; ++i)
		std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (((int)data[i]) & 0xFF) << " ";
	std::cout << std::endl;
	std::cout.flags(f);
}

std::vector<std::string> Message::split(const std::string &s, char delim)
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