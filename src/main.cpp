#include <iostream>
#include "Camera.h"
#include "CameraStandards.h"

// static Camera

int main()
{

	Camera *cam = new Camera("rtsp", "10.190.60.109:554", "root", "akts", CameraStandards::VIVOTEK_FD8136);

	// cam->showSnapShot();
	cam->showVideoStreamRTSP();

	delete(cam);
}