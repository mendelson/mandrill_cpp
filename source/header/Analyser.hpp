#pragma once

// #include "Observer.h"
// #include "FramesManager.h"
#include "Observer.hpp"

class Analyser : public Observer
{
public:
	Analyser();
	~Analyser();
	void Update();
	// void setSubject(FramesManager*
	// subject);

private:
	void showLastFrameIndex();
};

