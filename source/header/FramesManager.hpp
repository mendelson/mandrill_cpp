#pragma once

#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <bitset>    
#include <chrono> 
#include "Observer.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "Printer.hpp"

const unsigned int MAXFRAMES = 10;
const unsigned int TIMESPAN = 100; // miliseconds

typedef std::list<Observer*> observersList;
typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;
typedef std::unordered_map<unsigned int, std::shared_ptr<std::bitset<16>>> FramesMetadata;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

class FramesManager
{
public:
	static const unsigned int FPS;

	std::shared_ptr<cv::Mat> getFrame(unsigned int index);
	cv::Mat getLatestFrame();
	unsigned int getLatestFrameIndex();
	unsigned int getFrameAvailability(unsigned int frame);
	void setFrameAsBusy(unsigned int frameIndex);
	void setFrameAsFree(unsigned int frameIndex, unsigned int moduleIndex);
	void run();
	void setStreamSource(std::string url, std::string model);

	// Singleton pattern
	static FramesManager* getManager();
	FramesManager(FramesManager const&)  =  delete; // Desabling copy constructor
	void operator=(FramesManager const&) =  delete; // Desabling copy operator

	// Subject-observer pattern
	void attach(Observer*);
	void detach(Observer*);

	double getFramesWidth();
	double getFramesHeight();
	double getCameraFPS();
	const std::string getModel();

	bool lostCamera() { return _lostCamera; }

private:
	class BufferManager
	{
	public:
		BufferManager();
		void run();
		void stop();
		// void stopWhenEmpty();

	private:
		FramesManager* _framesManager;
		BufferManager(BufferManager const&)  =  delete; // Desabling copy constructor
		void operator=(BufferManager const&) =  delete; // Desabling copy operator

		bool _mustStop;
		// bool _mustStopWhenEmpty;
		std::mutex _stopMutex;
		bool _lostCamera;
	};

	void addFrame(cv::Mat frame);
	void Notify();
	unsigned int getNewId();

	FramesSet _framesSet;
	FramesMetadata _busyFrames;
	std::string _url;
	std::string _model;
	Camera* _camera;
	unsigned int _latestFrameIndex;
	observersList _observers;
	std::mutex _mutex;
	ThreadPool* _threadPool;
	static BufferManager* _bufferManager;
	bool _lostCamera;
	fsec timeDiff;
	ms miliDiff;


	// Singleton pattern
	static FramesManager* _instance;
	FramesManager();
	static std::mutex _instanceMutex;

	// Allowing parallelism
	// static void updateHelper(std::list<Observer*>::iterator it);

	void saveFrame(cv::Mat frame, cv::VideoWriter outputStream);

	static void bufferManagerRunHelper(FramesManager::BufferManager* bufferManager);
};
