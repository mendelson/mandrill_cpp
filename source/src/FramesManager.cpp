#include <iostream>
#include <thread>
// #include <memory>
// #include <opencv2/opencv.hpp>
// #include <unordered_map>
// #include <list>
// #include "Observer.hpp"
#include "FramesManager.hpp"

FramesManager* FramesManager::_instance = 0;

FramesManager* FramesManager::getManager()
{
	if (_instance == 0)
	{
		_instance = new FramesManager();
	}

	return _instance;
}

FramesManager::FramesManager()
{
	_latestFrame = -1;
}

void FramesManager::addFrame(cv::Mat frame)
{
	_latestFrame++;
	
	// std::cout << _latestFrame << std::endl;

	if(framesSet.size() >= MAXFRAMES)
	{
		framesSet.erase(_latestFrame - MAXFRAMES);
	}

	framesSet.emplace(_latestFrame, std::make_shared<cv::Mat>(frame));

	Notify();
}

cv::Mat FramesManager::getFrame(unsigned int index)
{
	return *framesSet[index];
}

cv::Mat FramesManager::getLatestFrame()
{
	return *framesSet[_latestFrame];
}

unsigned int FramesManager::getLatestFrameIndex()
{
	return _latestFrame;
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
		std::thread(FramesManager::updateHelper, it).detach();
		// threadsVector.push_back(std::thread(FramesManager::updateHelper, it));
		// threadsVector[threadsVector.size() - 1].detach();
		// (*it)->Update(this);
	}

	// if(threadsVector.size() == 0)
	// {
	// 	for(it = _observers.begin(); it != _observers.end(); it++)
	// 	{
	// 		threadsVector.push_back(std::thread(FramesManager::updateHelper, it));
	// 		threadsVector[threadsVector.size() - 1].detach();
	// 		// (*it)->Update(this);
	// 	}
	// }
}

void FramesManager::updateHelper(std::list<Observer*>::iterator it)
{
	(*it)->Update(FramesManager::getManager());
	// std::cout << "Ha!" << std::endl;
}