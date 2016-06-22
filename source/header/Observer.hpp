#pragma once

#include <iostream>
#include <mutex>
#include <string>

class FramesManager;

class Observer
{
public:
	Observer();
	virtual ~Observer() {}
	virtual void Update() = 0;
	virtual void setSubject(FramesManager *subject, unsigned int id);

protected:
	FramesManager *_subject;
	unsigned int _currentFrameIndex;
	std::mutex _mutex;
	unsigned int _id;
};

