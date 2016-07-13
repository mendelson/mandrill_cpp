#include "Core.hpp"
#define DEBUG

std::mutex printer_mutex;
bool movDetected;

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		std::cout << "It is mandatory to pass exactly the following "
					 "parameters: URL, camera_model and port_number! "
					 "Aborting..."
				  << std::endl;

		return -1;
	}

	Core *core = new Core(argv[1], argv[2], argv[3]);

	core->run();

	return 0;
}

