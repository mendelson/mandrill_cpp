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
	// void killThread(std::thread target);

	// std::vector<std::thread>* _threadVector;
	FramesManager* _framesManager;
};

#endif