#include <opencv2/opencv.hpp>
#include <thread>
#include <signal.h>
#include "Core.hpp"
#include "Analyser.hpp"
#include "Printer.hpp"
#include "Processor.hpp"

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

	Analyser* analyser = new Analyser();
	Printer* printer = new Printer();
	_framesManager->Attach(analyser);
	_framesManager->Attach(printer);

	std::thread framesManagerThread (framesManagerRunHelper, _framesManager);



	// std::string test = getAction();

	// joinAllThreads();
	// std::cout << "ha" << std::endl;

	std::cout << "hey" << std::endl;
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

// void Core::joinAllThreads()
// {
// 	for(unsigned int i = 0; i < (*_threadVector).size(); i++)
// 	{
// 		(*_threadVector)[i].join();
// 	}
// }
