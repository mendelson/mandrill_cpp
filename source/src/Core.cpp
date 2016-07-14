#include "Core.hpp"
#include "Analyser.hpp"
#include "CodecsConfig.hpp"
#include "GreyProcessor.hpp"
#include "MeanProcessor.hpp"
#include "MovementProcessor.hpp"
#include "Printer.hpp"
#include "Saver.hpp"
#include <opencv2/opencv.hpp>
#include <signal.h>
#include <thread>

static void framesManagerRunHelper(FramesManager *framesManager);
volatile sig_atomic_t flag = 0;

void handleInterruptionSignal(int sig)
{
	flag = 1;
}

Core::Core(std::string url, std::string model, std::string portNumber)
{
	CodecsConfig::update();
	_framesManager = FramesManager::getManager();
	_framesManager->setStreamSource(url, model);
	_socketCommunicator = SocketCommunicator::getSocket();
	_socketCommunicator->setupSocket(atoi(portNumber.c_str()));
}

void Core::run()
{
	signal(SIGINT, handleInterruptionSignal);

	// _socketCommunicator->run();

	// exit(-1999);
	return;

	// Observer* analyser = new
	// Analyser();
	// Observer* printer = new
	// Printer();
	// Observer* greyProcessor = new
	// GreyProcessor("H.264");
	Observer *saver = new Saver("H.264");
	// Observer* meanProcessor = new
	// MeanProcessor();
	Observer *moveProcessor = new MovementProcessor("H.264");

	// _framesManager->attach(analyser);
	// _framesManager->attach(printer);
	// _framesManager->attach(greyProcessor);
	_framesManager->attach(saver);
	// _framesManager->attach(meanProcessor);
	_framesManager->attach(moveProcessor);

	std::thread framesManagerThread(framesManagerRunHelper, _framesManager);

	while(true)
	{
		sleep(10);
		if(flag)
		{
			std::cout << "\n\nSIGINT caught!" << std::endl;
			std::cout << "\n\nExiting Core "
						 "smoothly..."
					  << std::endl;
			delete(ThreadPool::getThreadPool());
			flag = 0;
			break;
		}
	}

	framesManagerThread.join();

	if(_framesManager->lostCamera())
		exit(-6);
}

void framesManagerRunHelper(FramesManager *framesManager)
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

