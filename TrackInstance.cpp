#include "TrackInstance.h"
#include "Message.h"

TrackInstance::TrackInstance()
{


}

TrackInstance::TrackInstance(Message newMessage) {

	id = newMessage.trackInstance.id;
	range = newMessage.trackInstance.range;
	azimuth = newMessage.trackInstance.azimuth;
	velocity = newMessage.trackInstance.velocity;
	directionAngle = newMessage.trackInstance.directionAngle;
	radialVelocity = newMessage.trackInstance.velocity;
	status = newMessage.trackInstance.status;
	classificationStatus = newMessage.trackInstance.classificationStatus;
	snr = newMessage.trackInstance.snr;

	latitude = newMessage.trackInstance.latitude;
	longitude = newMessage.trackInstance.longitude;
	trackTime = newMessage.trackInstance.trackTime;
	trackRadarID = newMessage.trackInstance.trackRadarID;
	/*if (newMessage.radar.id==1)
	{
		trackRadarID = 1;
	}
	else if(newMessage.radar.id == 2)
	{
		trackRadarID = 2;
	}*/
	localTime = newMessage.trackInstance.localTime;
	time = newMessage.trackInstance.time;
	isFused = false;
}
