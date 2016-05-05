#include <opencv2/opencv.hpp>
#include <thread>
#include <signal.h>
#include "Core.hpp"
#include "Analyser.hpp"
#include "Printer.hpp"
#include "GreyProcessor.hpp"
#include "Saver.hpp"

static void framesManagerRunHelper(FramesManager* framesManager);
volatile sig_atomic_t flag = 0;

void handleInterruptionSignal(int sig)
{
	flag = 1;
}

Core::Core(std::string url, std::string model)
{
	_framesManager = FramesManager::getManager();
	_framesManager->setStreamSource(url, model);
}

void Core::run()
{
	signal(SIGINT, handleInterruptionSignal);

	// Observer* analyser = new Analyser();
	// Observer* printer = new Printer();
	Observer* greyProcessor = new GreyProcessor();
	Observer* saver = new Saver();
	// _framesManager->Attach(analyser);
	// _framesManager->Attach(printer);
	_framesManager->Attach(greyProcessor);
	_framesManager->Attach(saver);

	std::thread framesManagerThread (framesManagerRunHelper, _framesManager);

	while(true)
	{
		if(flag)
		{
			std::cout << "\n\nSIGINT caught!" << std::endl;
			std::cout << "\n\nExiting Core smoothly..." << std::endl;
			delete(ThreadPool::getThreadPool());
			flag = 0;
			break;
		}
	}

	framesManagerThread.join();
}

void framesManagerRunHelper(FramesManager* framesManager)
{
	framesManager->run();
}

std::string Core::getAction()
{
	std::string action;

	getline(std::cin, action);

	std::cout << action << std::endl;

	return action;
}
