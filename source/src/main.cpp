#include "Core.hpp"
#define DEBUG

std::mutex printer_mutex;
bool movDetected;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "It is mandatory to "
					 "pass exactly the "
					 "following "
					 "parameters: URL and "
					 "camera_model! "
					 "Aborting..."
				  << std::endl;

		return -1;
	}

	Core *core = new Core(argv[1], argv[2]);

	core->run();

	return 0;
}

