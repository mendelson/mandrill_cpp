#pragma once

#include <mutex>
#include <iostream>
#include <string>

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
	std::mutex _mutex;
};
