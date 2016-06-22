#include "FramesManager.hpp"
#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>

FramesManager *FramesManager::_instance = 0;
FramesManager::BufferManager *FramesManager::_bufferManager = 0;
std::mutex FramesManager::_instanceMutex;
const unsigned int FramesManager::FPS = 10;

FramesManager *FramesManager::getManager()
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

	std::bitset<16> busyVector;
	// std::cout << _latestFrameIndex << std::endl;

	// if(_framesSet.size() >= MAXFRAMES)
	// {
	// 	_framesSet.erase(_latestFrameIndex + 1 - MAXFRAMES);
	// }

	_framesSet.emplace(_latestFrameIndex + 1, std::make_shared<cv::Mat>(frame));
	_busyFrames.emplace(_latestFrameIndex + 1,
						std::make_shared<std::bitset<16>>(busyVector));

	_latestFrameIndex++;
	setFrameAsBusy(_latestFrameIndex);
	Printer::safe_print("MANAGER:" + std::to_string(_latestFrameIndex));

	// Printer::safe_print("Saver: " + std::to_string(_latestFrameIndex));

	_mutex.unlock();

	Notify();
}

std::shared_ptr<cv::Mat> FramesManager::getFrame(unsigned int index)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>>::const_iterator
		element = _framesSet.find(index);

	if (element == _framesSet.end())
		return NULL;
	else
		return std::make_shared<cv::Mat>(*element->second);
}

cv::Mat FramesManager::getLatestFrame()
{
	std::unique_lock<std::mutex> _lock(_mutex);
	return *_framesSet[_latestFrameIndex];
}

unsigned int FramesManager::getFrameAvailability(unsigned int frameIndex)
{
	// std::unique_lock<std::mutex> _lock(_mutex);
	std::bitset<16> busyVector;

	busyVector = *_busyFrames[frameIndex];
	if (busyVector.none())
		return 0;
	else
		return 1;
}

unsigned int FramesManager::getLatestFrameIndex()
{
	return _latestFrameIndex;
}

void FramesManager::run()
{
	if (_url.compare("") == 0 || _model.compare("") == 0)
	{
		std::cout << "You have not set the address and model of your streaming "
					 "camera!" << std::endl;

		exit(1);
	}

	_bufferManager = new BufferManager();
	std::thread bufferManagerThread(bufferManagerRunHelper, _bufferManager);

	unsigned int frameCounter = 0;
	unsigned int frameGap = _camera->getFps() / FPS;

	while (true)
	{
		if (ThreadPool::mustStop())
		{
			_bufferManager->stop();
			break;
		}

		_camera->updateFrame();
		// std::cout << "FPS: " << _camera->getFps() << std::endl;

		cv::Mat frame = _camera->getFrame().clone();

		if (!frame.empty())
		{
			// cv::imshow("Live streaming from " + _camera->getIp(), frame);
			std::cout << std::endl
					  << "Frame GAP: " << frameGap << std::endl;
			std::cout << "Frame counter: " << frameCounter << std::endl;
			std::cout << "Intervalo: " << frameCounter % frameGap << std::endl
					  << std::endl;

			if (!ThreadPool::mustStop())
			{
				if (frameCounter % frameGap == 0)
				{
					addFrame(frame);
					frameCounter = 0;
				}
				else
				{
					frame.release();
					std::cout << "Dropando: " << frameCounter << std::endl;
				}

				frameCounter++;
				// for(i = 0; i < FPS; i++)
				// {
				// 	addFrame(frame);
				// }

				// std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				// saveFrame(frame, outputStream);
			}
		}
		// Must reconnect to the streaming source
		else
		{
			// std::cout << "---------------------------------------------------
			// EMPTY --------------------------------------" << std::endl;

			// _camera->reconnect();
			_lostCamera = true;
			break;
		}

		// Sleeps to get a lower frame rate
		// std::this_thread::sleep_for (std::chrono::milliseconds(1000));
	}

	_bufferManager->stop();
	bufferManagerThread.join();
}

void FramesManager::setStreamSource(std::string url, std::string model)
{
	_url = url;
	_model = model;

	_camera = new Camera(_url, _model);
}

void FramesManager::attach(Observer *newObserver)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	_observers.push_back(newObserver);
	newObserver->setSubject(this, getNewId());
}

void FramesManager::detach(Observer *observer)
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

const std::string FramesManager::getModel()
{
	return _model;
}

void FramesManager::Notify()
{
	std::unique_lock<std::mutex> _lock(_mutex);

	std::list<Observer *>::iterator it;

	for (it = _observers.begin(); it != _observers.end(); it++)
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

void FramesManager::bufferManagerRunHelper(
	FramesManager::BufferManager *bufferManager)
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
	// _mustStopWhenEmpty = false;
}

void FramesManager::BufferManager::run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMESPAN));

		if (_mustStop)
		{
			_framesManager->_framesSet.clear();
			_framesManager->_busyFrames.clear();
			break;
		}

		auto it = _framesManager->_busyFrames.begin();
		auto previous_it = it;
		unsigned int currentKey;

		_framesManager->_mutex.lock();
		// std::cout << "lockei!!!" << std::endl;
		while (it != _framesManager->_busyFrames.end())
		{
			previous_it = it;
			it++;

			if (_framesManager->getFrameAvailability(previous_it->first) == 0)
			{
				std::cout << "BIURLLLL " << previous_it->first << std::endl;

				currentKey = previous_it->first;

				_framesManager->_framesSet.erase(currentKey);
				_framesManager->_busyFrames.erase(currentKey);
			}
		}
		_framesManager->_mutex.unlock();

		std::cout << "=============== BufferManager ==============="
				  << std::endl;

		// if(_mustStopWhenEmpty && _framesManager->_framesSet.size() == 0)
		// break;
	}
}

void FramesManager::BufferManager::stop()
{
	_mustStop = true;
}

// void FramesManager::BufferManager::stopWhenEmpty()
// {
// 	_mustStopWhenEmpty = true;
// }

void FramesManager::setFrameAsBusy(unsigned int frameIndex)
{
	// std::unique_lock<std::mutex> _lock(_mutex);

	auto it = _busyFrames.find(frameIndex);

	if (it != _busyFrames.end())
	{
		// it->second->set(moduleIndex, 1);

		for (unsigned int i = 0; i < _observers.size(); i++)
		{
			it->second->set(i, 1);
		}
	}
}

void FramesManager::setFrameAsFree(unsigned int frameIndex,
								   unsigned int moduleIndex)
{
	std::unique_lock<std::mutex> _lock(_mutex);

	auto it = _busyFrames.find(frameIndex);

	if (it != _busyFrames.end())
		it->second->set(moduleIndex, 0);
}

