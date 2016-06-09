#pragma once

#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include "Observer.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "Printer.hpp"

const unsigned int MAXFRAMES = 10;

typedef std::list<Observer*> observersList;
typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;

class FramesManager
{
public:
	std::shared_ptr<cv::Mat> getFrame(unsigned int index);
	cv::Mat getLatestFrame();
	unsigned int getLatestFrameIndex();
	void run();
	void setStreamSource(std::string url, std::string model);

	// Singleton pattern
	static FramesManager* getManager();
	FramesManager(FramesManager const&)  =  delete; // Desabling copy constructor
	void operator=(FramesManager const&) =  delete; // Desabling copy operator

	// Subject-observer pattern
	void Attach(Observer*);
	void Detach(Observer*);

	double getFramesWidth();
	double getFramesHeight();
	double getCameraFPS();

private:
	void addFrame(cv::Mat frame);
	void Notify();
	unsigned int getNewId();

	FramesSet _framesSet;
	std::string _url;
	std::string _model;
	Camera* _camera;
	unsigned int _latestFrameIndex;
	observersList _observers;
	std::mutex _mutex;
	ThreadPool* _threadPool;

	// Singleton pattern
	static FramesManager* _instance;
	FramesManager();
	static std::mutex _instaceMutex;

	// Allowing parallelism
	// static void updateHelper(std::list<Observer*>::iterator it);


	void saveFrame(cv::Mat frame, cv::VideoWriter outputStream);
};
