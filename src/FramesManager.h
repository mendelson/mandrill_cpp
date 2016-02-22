#ifndef FRAMES_MANAGER_H
#define FRAMES_MANAGER_H

#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <list>
#include "Observer.h"

const unsigned int MAXFRAMES = 10;

typedef std::list<Observer*> observersList;
typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;

class FramesManager// : public Subject
{
public:
	static FramesManager* Instance();
	void addFrame(cv::Mat frame);
	cv::Mat getFrame(unsigned int index);
	cv::Mat getLatestFrame();
	unsigned int getLatestFrameIndex();
	void Attach(Observer*);
	void Detach(Observer*);
	void Notify();

protected:
	FramesManager();
	FramesSet framesSet;

private:
	static FramesManager* _instance;
	unsigned int _latestFrame;
	observersList _observers;
};

#endif