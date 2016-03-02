#include <opencv2/opencv.hpp>
#include <thread>
#include "Core.hpp"

static void framesManagerRunHelper(FramesManager* framesManager);

Core::Core(std::string url, std::string model)
{
	FramesManager* framesManager = FramesManager::getManager();
	framesManager->setStreamSource(url, model);
}

void Core::run()
{
	// Analyser* anal = new Analyser();
	// Printer* printer = new Printer();
	// framesManager->Attach(anal);
	// framesManager->Attach(printer);
	
	std::cout << "ha" << std::endl;
	threadVector->emplace_back(std::thread(framesManagerRunHelper, framesManager));
	std::cout << "ha" << std::endl;
	// (*threadVector)[0].join();
	// std::cout << "ha" << std::endl;


	// std::string test = getAction();

	// std::cout << "ha" << std::endl;

	joinAllThreads();
	std::cout << "ha" << std::endl;
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

void Core::killThread(std::thread target)
{
	// TODO
}

void Core::joinAllThreads()
{
	std::cout << "ha" << std::endl;

	for(unsigned int i = 0; i < (*threadVector).size(); i++)
	{
		(*threadVector)[i].join();
	}

	std::cout << "ha" << std::endl;
}