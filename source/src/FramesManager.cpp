#include <iostream>
#include <thread>
// #include <memory>
// #include <opencv2/opencv.hpp>
// #include <unordered_map>
// #include <list>
// #include "Observer.hpp"
#include "FramesManager.hpp"

FramesManager* FramesManager::_instance = 0;
std::mutex FramesManager::_instaceMutex;

FramesManager* FramesManager::getManager()
{
	std::unique_lock<std::mutex> _lock(_instaceMutex);

	if (_instance == 0)
	{
		_instance = new FramesManager();
	}
	
	return _instance;
}


FramesManager::FramesManager()
{
	_url = "";
	_model = "";
	_latestFrame = -1;
	_camera = NULL;
	_threadPool = ThreadPool::getThreadPool();
}

void FramesManager::addFrame(cv::Mat frame)
{
	_mutex.lock();

	_latestFrame++;
	
	// std::cout << _latestFrame << std::endl;

	if(_framesSet.size() >= MAXFRAMES)
	{
		_framesSet.erase(_latestFrame - MAXFRAMES);
	}

	_framesSet.emplace(_latestFrame, std::make_shared<cv::Mat>(frame));
	
	_mutex.unlock();

	Notify();
}

cv::Mat FramesManager::getFrame(unsigned int index)
{
	return *_framesSet[index];
}

cv::Mat FramesManager::getLatestFrame()
{
	return *_framesSet[_latestFrame];
}

unsigned int FramesManager::getLatestFrameIndex()
{
	return _latestFrame;
}

void FramesManager::run()
{
	if(_url.compare("") == 0 || _model.compare("") == 0)
	{
		std::cout << "You have not set the address and model of your streaming camera!" << std::endl;

		exit(1);
	}

	while(true)
	{
		if(ThreadPool::mustStop())
			break;

		_camera->updateFrame();

		cv::Mat frame = _camera->getFrame().clone();

		if(!frame.empty())
		{
			// cv::imshow("Live streaming from " + _camera->getIp(), frame);

			if(!ThreadPool::mustStop())
				addFrame(frame);
		}

		// // "ESC" key aborts execution
		// char pressedKey = cv::waitKey(30);
		// // std::cout << (int) pressedKey << std::endl;
		// if (pressedKey == 27) break;
	}
}

void FramesManager::setStreamSource(std::string url, std::string model)
{
	_url = url;
	_model = model;
	
	_camera = new Camera(_url, _model);
}

void FramesManager::Attach(Observer* newObserver)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	newObserver->setSubject(this);
	_observers.push_back(newObserver);
}

void FramesManager::Detach(Observer* observer)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	_observers.remove(observer);
}

void FramesManager::Notify()
{
	std::unique_lock<std::mutex> _lock(_mutex);

	std::list<Observer*>::iterator it;

	for(it = _observers.begin(); it != _observers.end(); it++)
	{
		// TODO: thread pool instead of the following gambiarra
		// std::thread(FramesManager::updateHelper, it).detach();
		_threadPool->enqueue(*it);
	}
}

// void FramesManager::updateHelper(std::list<Observer*>::iterator it)
// {
// 	// (*it)->Update(FramesManager::getManager());
// 	(*it)->Update();
// }