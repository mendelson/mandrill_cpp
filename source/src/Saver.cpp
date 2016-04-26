#include <iostream>
#include "Saver.hpp"
#include "FramesManager.hpp"

const double FPSSAVER = 20;

Saver::~Saver()
{
	_outputStream->release();
	delete _outputStream;
	std::cout << "wow!" << std::endl;
}

void Saver::Update()
{
  // auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
  getCurrentFrame();

	std::cout << "Saver: " << _currentFrameIndex << "|" << std::endl;

	saveFrame();
}

void Saver::setSubject(FramesManager* subject)
{
	this->_subject = subject;
	_outputStream = new cv::VideoWriter("data/streaming/originalStream.avi",
 								 CV_FOURCC('M','P','E','G'),
 								 FPSSAVER,
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

void Saver::saveFrame()
{
	*_outputStream << *_frame;
}
