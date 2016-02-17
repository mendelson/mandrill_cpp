#ifndef FRAMES_MANAGER_H
#define FRAMES_MANAGER_H

#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>

const unsigned int MAXFRAMES = 10;

typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;

class FramesManager
{
public:
	static FramesManager* Instance();
	void addFrame(cv::Mat frame);
	cv::Mat getFrame(unsigned int index);

protected:
	FramesManager();
	FramesSet framesSet;

private:
	static FramesManager* _instance;
	unsigned int latestFrame;
};

#endif

// Ver Observer!!!