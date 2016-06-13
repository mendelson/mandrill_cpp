#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include "FramesManager.hpp"

FramesManager* FramesManager::_instance = 0;
FramesManager::BufferManager* FramesManager::_bufferManager = 0;
std::mutex FramesManager::_instanceMutex;

FramesManager* FramesManager::getManager()
{
	std::unique_lock<std::mutex> _lock(_instanceMutex);

	if (_instance == 0)
	{
		_instance = new FramesManager();
	}

	return _instance;
}

FramesManager::FramesManager() : _lostCamera(false)
{
	_url = "";
	_model = "";
	_latestFrameIndex = -1;
	_camera = NULL;
	_threadPool = ThreadPool::getThreadPool();
}

void FramesManager::addFrame(cv::Mat frame)
{
	_mutex.lock();

	// std::cout << _latestFrameIndex << std::endl;

	if(_framesSet.size() >= MAXFRAMES)
	{
		_framesSet.erase(_latestFrameIndex + 1 - MAXFRAMES);
	}

	_framesSet.emplace(_latestFrameIndex + 1, std::make_shared<cv::Mat>(frame));

	_latestFrameIndex++;
	Printer::safe_print("MANAGER:" + std::to_string(_latestFrameIndex) );

	// Printer::safe_print("Saver: " + std::to_string(_latestFrameIndex));


	_mutex.unlock();

	Notify();
}

std::shared_ptr<cv::Mat> FramesManager::getFrame(unsigned int index)
{
	std::unique_lock<std::mutex> _lock(_mutex);

  	std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>>::const_iterator element = _framesSet.find(index);

	if(element == _framesSet.end())
		return NULL;
	else
		return std::make_shared<cv::Mat>(*element->second);
}

cv::Mat FramesManager::getLatestFrame()
{
	std::unique_lock<std::mutex> _lock(_mutex);
	return *_framesSet[_latestFrameIndex];
}

unsigned int FramesManager::getLatestFrameIndex()
{
	return _latestFrameIndex;
}

void FramesManager::run()
{
	if(_url.compare("") == 0 || _model.compare("") == 0)
	{
		std::cout << "You have not set the address and model of your streaming camera!" << std::endl;

		exit(1);
	}

	_bufferManager = new BufferManager();
	std::thread bufferManagerThread (bufferManagerRunHelper, _bufferManager);

	while(true)
	{
		if(ThreadPool::mustStop())
		{
			_bufferManager->stop();
			break;
		}

		_camera->updateFrame();
		// std::cout << "FPS: " << _camera->getFps() << std::endl;

		cv::Mat frame = _camera->getFrame().clone();

		if(!frame.empty())
		{
			// cv::imshow("Live streaming from " + _camera->getIp(), frame);

			if(!ThreadPool::mustStop())
			{
				addFrame(frame);
				// saveFrame(frame, outputStream);
			}
		}
		// Must reconnect to the streaming source
		else
		{
			// std::cout << "--------------------------------------------------- EMPTY --------------------------------------" << std::endl;
			
			// _camera->reconnect();
			_lostCamera = true;
			break;
		}
	}

	_bufferManager->stopWhenEmpty();
	bufferManagerThread.join();
}

void FramesManager::setStreamSource(std::string url, std::string model)
{
	_url = url;
	_model = model;

	_camera = new Camera(_url, _model);
}

void FramesManager::attach(Observer* newObserver)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	_observers.push_back(newObserver);
	newObserver->setSubject(this, getNewId());
}

void FramesManager::detach(Observer* observer)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	_observers.remove(observer);
}

double FramesManager::getFramesWidth()
{
	return _camera->getWidth();
}

double FramesManager::getFramesHeight()
{
	return _camera->getHeight();
}

double FramesManager::getCameraFPS()
{
	return _camera->getFps();
}

void FramesManager::Notify()
{
	std::unique_lock<std::mutex> _lock(_mutex);

	std::list<Observer*>::iterator it;

	for(it = _observers.begin(); it != _observers.end(); it++)
	{
		// std::thread(FramesManager::updateHelper, it).detach();
		_threadPool->enqueue(*it);
	}
}

unsigned int FramesManager::getNewId()
{
	return _observers.size() - 1;
}

// void FramesManager::updateHelper(std::list<Observer*>::iterator it)
// {
// 	// (*it)->Update(FramesManager::getManager());
// 	(*it)->Update();
// }

void FramesManager::saveFrame(cv::Mat frame, cv::VideoWriter outputStream)
{
	outputStream << frame;
}

void FramesManager::bufferManagerRunHelper(FramesManager::BufferManager* bufferManager)
{
	bufferManager->run();
}

/*****************
*  BufferManager *
*****************/

FramesManager::BufferManager::BufferManager()
{
	_framesManager = FramesManager::getManager();
	_mustStop = false;
	_mustStopWhenEmpty = false;
}

void FramesManager::BufferManager::run()
{
	while(true)
	{
		std::this_thread::sleep_for (std::chrono::milliseconds(TIMESPAN));

		if(_mustStop)
			break;

		//DO STUFF
		std::cout << "=============== BufferManager ===============" << std::endl;

		if(_mustStopWhenEmpty && _framesManager->_framesSet.size() == 0)
			break;
	}
}

void FramesManager::BufferManager::stop()
{
	_mustStop = true;
}

void FramesManager::BufferManager::stopWhenEmpty()
{
	_mustStopWhenEmpty = true;
}