#ifndef OBSERVER_H
#define OBSERVER_H

// #include "FramesManager.hpp"

class FramesManager;

class Observer
{
public:
	virtual void Update(FramesManager* theChangedSubject) = 0;
	virtual void setSubject(FramesManager* subject) = 0;

protected:
	FramesManager* _subject;
};

#endif