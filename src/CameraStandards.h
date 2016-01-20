#ifndef CAMERA_STANDARDS_H
#define CAMERA_STANDARDS_H

#include <iostream>

class CameraStandards
{
public:
	enum MODEL
	{
		VIVOTEK_FD8136,
		FOO
	};

	CameraStandards(){}
	static std::string getSnapShotPath(const CameraStandards::MODEL&);
	static std::string getVideoStreamPath(const CameraStandards::MODEL&);
};

#endif