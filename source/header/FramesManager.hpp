#ifndef FRAMES_MANAGER_H
#define FRAMES_MANAGER_H

#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include "Observer.hpp"
#include "Camera.hpp"

const unsigned int MAXFRAMES = 10;

typedef std::list<Observer*> observersList;
typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;

class FramesManager
{
public:
	cv::Mat getFrame(unsigned int index);
	cv::Mat getLatestFrame();
	unsigned int getLatestFrameIndex();
	void run();
	void setStreamSource(std::string url, std::string model);
	

	// Singleton pattern
	static FramesManager* getManager();

	// Subject-observer pattern
	void Attach(Observer*);
	void Detach(Observer*);

protected:
	FramesSet framesSet;
	
	// Singleton pattern
	FramesManager();

private:
	void addFrame(cv::Mat frame);
	void Notify();

	std::string _url;
	std::string _model;
	Camera* _camera;
	unsigned int _latestFrame;
	observersList _observers;
	std::vector<std::thread> _threadsVector;
	std::mutex _mutex;

	// Singleton pattern
	static FramesManager* _instance;

	// Allowing parallelism
	static void updateHelper(std::list<Observer*>::iterator it);
};

#endif