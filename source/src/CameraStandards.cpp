#include "CameraStandards.h"
#include <iostream>

std::string
CameraStandards::getSnapShotPath(const CameraStandards::MODEL &model)
{
	switch(model)
	{
		case VIVOTEK_FD8136:
			return "cgi-bin/viewer/"
				   "video.jpg";
			break;
		case AXIS_M1014:
			return "axis-cgi/jpg/"
				   "image.cgi";
			break;
		default:
			return "invalid_model";
			break;
	}
}

std::string
CameraStandards::getVideoStreamPath(const CameraStandards::MODEL &model)
{
	switch(model)
	{
		case VIVOTEK_FD8136:
			return "live.sdp";
			break;
		case AXIS_M1014:
			return "axis-media/"
				   "media.amp";
			break;
		default:
			return "invalid_model";
			break;
	}
}

