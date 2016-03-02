#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <vector>
#include "FramesManager.hpp"

class Core
{
public:
	void run();
	Core(std::string url, std::string model);

private:
	std::string getAction();
	void killThread(std::thread target);
	void joinAllThreads();

	std::vector<std::thread>* threadVector;
	FramesManager* framesManager;
};

#endif