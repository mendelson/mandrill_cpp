#include "Core.hpp"
#include "GstCore.hpp"
#include "gst/gst.h"
#define DEBUG

std::mutex printer_mutex;
bool movDetected;

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		std::cout << "It is mandatory to pass exactly the following "
					 "parameters: UrlHighQuality UrlLowQuality "
					 "UUID PathToSave"
				  << std::endl << "Aborting..." << std::endl;

		return -1;
	}
/*
	Core *core = new Core(argv[1], argv[2], argv[3]);

	core->run();
*/
	/* Initialize GStreamer */
	gst_init(&argc, &argv);
    GstCore *gstCore = new GstCore(argv[1], argv[2], argv[3], argv[4]);
    gstCore->run();	

    return 0;
}

