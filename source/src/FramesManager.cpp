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
		// _instance->nha = 123;
		// std::cout << "nha0 " << _instance->nha << std::endl;
		// std::cout << "Mudou!" << std::endl;
	}
	// std::cout << "nha01 " << _instance->nha << std::endl;
	// else
	// {
		// std::cout << "Nao mudou!" << std::endl;
	// }

	// std::cout << "getManager: " << _instance << std::endl;
	
	return _instance;
}


FramesManager::FramesManager()
{
	_url = "";
	_model = "";
	_latestFrame = -1;
	_camera = NULL;
}

void FramesManager::addFrame(cv::Mat frame)
{
	_mutex.lock();

	_latestFrame++;
	
	// std::cout << _latestFrame << std::endl;

	if(framesSet.size() >= MAXFRAMES)
	{
		framesSet.erase(_latestFrame - MAXFRAMES);
	}

	framesSet.emplace(_latestFrame, std::make_shared<cv::Mat>(frame));
	
	_mutex.unlock();

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

void FramesManager::run()
{
	// std::cout << 1 << std::endl;

	std::cout << "hey run!" << std::endl;


	// std::cout << _url << std::endl;
	// if(_url.compare("") == 0 || _url.compare("") == 0)
	// {
	// 	std::cout << "You have not set the address and model of your streaming camera!" << std::endl;

	// 	exit(1);
	// }

	// std::cout << 2 << std::endl;

	// while(true)
	// {
	// 	_camera->updateFrame();

	// 	cv::Mat frame = _camera->getFrame().clone();

	// 	if(!frame.empty())
	// 	{
	// 		// cv::imshow("Live streaming from " + camera->getIp(), frame);

	// 		addFrame(frame);
	// 	}

	// 	// "ESC" key aborts execution
	// 	char pressedKey = cv::waitKey(30);
	// 	// std::cout << (int) pressedKey << std::endl;
	// 	if (pressedKey == 27) break;
	// }
}

void FramesManager::setStreamSource(std::string url, std::string model)
{
	_url = url;
	_model = model;
	
	_camera = new Camera(_url, _model);
	// std::cout << "Manolo" << std::endl;
	// std::cout << nha << std::endl;
	// nha = 10;
	// std::cout << "nha na definicao: " << nha << std::endl;
	// std::cout << "Manolo 2" << std::endl;
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