#include <iostream>
#include "GreyProcessor.hpp"
#include "FramesManager.hpp"

GreyProcessor::~GreyProcessor()
{
	_outputStream->release();
	delete _outputStream;
	std::cout << "wow!" << std::endl;
}

void GreyProcessor::Update()
{
  // auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
  	getCurrentFrame();

	cv::Mat oneChannelGreyFrame;

	cv::cvtColor(*_frame, oneChannelGreyFrame, CV_RGB2GRAY);
	cv::cvtColor(oneChannelGreyFrame, _greyFrame, CV_GRAY2RGB);

	std::cout << "GreyProcessor: " << _currentFrameIndex << "|" << std::endl;

	saveFrame(_greyFrame);
  // saveFrame(*_frame);
}

void GreyProcessor::setSubject(FramesManager* subject)
{
	this->_subject = subject;
	_outputStream = new cv::VideoWriter("data/streaming/greyStream.avi",
 								 CV_FOURCC('X', '2', '6', '4'),
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

// void GreyProcessor::saveFrame(cv::Mat frame, cv::VideoWriter* outputStream)
void GreyProcessor::saveFrame(cv::Mat frame)
{
	*_outputStream << frame;
}
