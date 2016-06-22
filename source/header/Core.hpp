#pragma once

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
	FramesManager *_framesManager;
};

