#include "GstCore.hpp"
#include <signal.h>
#include <thread>
#include <unistd.h>


volatile sig_atomic_t flag = 0;

void handleInterruptionSignal(int sig)
{
	flag = 1;
}

GstCore::GstCore(std::string urlH, std::string urlL, std::string uuid,
				 std::string path)
{
	gstManager = new GstManager(urlH, urlL, uuid, path);
}

void GstCore::run()
{
	//	signal(SIGINT, handleInterruptionSignal);

	gstManager->run();
	while(true)
	{
		sleep(10);
		if(flag)
		{
			std::cout << "\n\nSIGINT caught!" << std::endl;
			std::cout << "\n\nExiting GstCore smoothly..." << std::endl;
			flag = 0;
			break;
		}
	}
}

