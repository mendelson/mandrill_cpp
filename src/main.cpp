#include <iostream>
#include "Camera.h"
#include "CameraStandards.h"

// static Camera

int main()
{

	Camera *cam = new Camera("http", "10.190.60.113", "root", "akts", CameraStandards::VIVOTEK_FD8136);

	cam->getSnapShot();
	// cam->showVideoStream();

	delete(cam);
}