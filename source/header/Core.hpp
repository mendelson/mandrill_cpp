#pragma once

#include "FramesManager.hpp"
#include <iostream>
#include <vector>

class Core
{
public:
	void run();
	Core(std::string url, std::string model);

private:
	std::string getAction();
	FramesManager *_framesManager;
};

