#include "MovementDetector.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	MovementDetector *md = new MovementDetector();
	char file1[]		 = "video12.mp4";
	char file2[]		 = "video13.mp4";
	char file3[]		 = "video14.mp4";
	char file4[]		 = "video15.mp4";
	char file5[]		 = "video16.mp4";

	std::cout << file1 << ": " << md->detectMovement(file1) << std::endl;
	std::cout << file2 << ": " << md->detectMovement(file2) << std::endl;
	std::cout << file3 << ": " << md->detectMovement(file3) << std::endl;
	std::cout << file4 << ": " << md->detectMovement(file4) << std::endl;
	std::cout << file5 << ": " << md->detectMovement(file5) << std::endl;

	delete md;

	return 0;
}

