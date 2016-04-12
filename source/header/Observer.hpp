#ifndef OBSERVER_H
#define OBSERVER_H

#include <mutex>
// #include "FramesManager.hpp"

class FramesManager;

class Observer
{
public:
	virtual void Update() = 0;
	void setSubject(FramesManager* subject) {this->_subject = subject;};

protected:
	FramesManager* _subject;
	unsigned int _currentFrame;
	// bool locked;
	std::mutex _mutex;
};

#endif
