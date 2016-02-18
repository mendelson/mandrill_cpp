#include <iostream>
#include <memory>
#include "FramesManager.h"

FramesManager* FramesManager::_instance = 0;

FramesManager* FramesManager::Instance()
{
	if (_instance == 0)
	{
		_instance = new FramesManager();
	}

	return _instance;
}

FramesManager::FramesManager()
{
	latestFrame = -1;
}

void FramesManager::addFrame(cv::Mat frame)
{
	latestFrame++;
	
	// std::cout << latestFrame << std::endl;

	if(framesSet.size() >= MAXFRAMES)
	{
		framesSet.erase(latestFrame - MAXFRAMES);
	}

	framesSet.emplace(latestFrame, std::make_shared<cv::Mat>(frame));

	Notify();
}

cv::Mat FramesManager::getFrame(unsigned int index)
{
	return *framesSet[index];
}

cv::Mat FramesManager::getLatestFrame()
{
	return *framesSet[latestFrame];
}

void FramesManager::Attach(Observer* newObserver)
{
	newObserver->setSubject(this);
	_observers.push_back(newObserver);
}

void FramesManager::Detach(Observer* observer)
{
	_observers.remove(observer);
}

void FramesManager::Notify()
{
	std::list<Observer*>::iterator it;

	for(it = _observers.begin(); it != _observers.end(); it++)
	{
		(*it)->Update(this);
	}
}