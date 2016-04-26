#pragma once

#include <mutex>
#include <iostream>
// #include "FramesManager.hpp"

class FramesManager;

class Observer
{
public:
	Observer();
	virtual ~Observer(){}
	virtual void Update() = 0;
	virtual void setSubject(FramesManager* subject) {this->_subject = subject;}

protected:
	FramesManager* _subject;
	unsigned int _currentFrameIndex;
	// bool locked;
	std::mutex _mutex;
};
