#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <thread>
#include "FramesManager.hpp"

FramesManager* FramesManager::_instance = 0;
std::mutex FramesManager::_instaceMutex;

// void saveFrame(cv::Mat frame);

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


	// double fps = 30;
	// cv::VideoWriter outputStream("streaming/testStream.avi", CV_FOURCC('M','P','E','G'), fps, cvSize((int)_camera->getWidth(),(int)_camera->getHeight()));

	while(true)
	{
		if(ThreadPool::mustStop())
			break;

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

// void FramesManager::updateHelper(std::list<Observer*>::iterator it)
// {
// 	// (*it)->Update(FramesManager::getManager());
// 	(*it)->Update();
// }

void FramesManager::saveFrame(cv::Mat frame, cv::VideoWriter outputStream)
{
	// cv::Mat _greyFrame;
	// cv::cvtColor(frame, _greyFrame, CV_RGB2GRAY);
	// cv::cvtColor(_greyFrame, frame, CV_GRAY2RGB);

	// std::cout << _greyFrame.cols << " | " << _greyFrame.rows << std::endl;
	// std::cout << frame.channels() << " | " << _greyFrame.channels() << std::endl;

	// outputStream << _greyFrame;
	outputStream << frame;

	// std::cout << "width = " << _camera->getWidth() << "     " << "height = "<< _camera->getHeight() << std::endl;
}
