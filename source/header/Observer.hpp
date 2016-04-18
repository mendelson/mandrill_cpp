#pragma once

#include <mutex>
// #include "FramesManager.hpp"

class FramesManager;

class Observer
{
public:
	Observer();
	virtual void Update() = 0;
	void setSubject(FramesManager* subject) {this->_subject = subject;};

protected:
	FramesManager* _subject;
	unsigned int _currentFrameIndex;
	// bool locked;
	std::mutex _mutex;
};
