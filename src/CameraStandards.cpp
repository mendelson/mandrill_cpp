#include <iostream>
#include "CameraStandards.h"

std::string CameraStandards::getSnapShotPath(const CameraStandards::MODEL& model)
{
	switch(model)
	{
		case VIVOTEK_FD8136:
			return "cgi-bin/viewer/video.jpg";
			break;
		case FOO:
			return "FOOOOOOOOO!!!!!";
			break;
		default:
			return "invalid_model";
			break;
	}
}

std::string CameraStandards::getVideoStreamPath(const CameraStandards::MODEL& model)
{
	switch(model)
	{
		case VIVOTEK_FD8136:
			return "live.sdp";
			break;
		case FOO:
			return "FOOOOOOOOO STREAM!!!!!";
			break;
		default:
			return "invalid_model";
			break;
	}
}