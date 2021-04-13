#ifndef TRACKINSTANCE
#define TRACKINSTANCE


class Message;

class TrackInstance {
public:
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
	unsigned long long time;
	unsigned long long localTime;
	bool isFused;
	unsigned long long trackTime;
	int fuseStatus; // 0 :  tespit  , 1 : fuse , 2:drop
	int secondId;
	int trackRadarID;

	TrackInstance();


	TrackInstance(Message newMessage);






};

#endif