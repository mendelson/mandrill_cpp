#include <iostream>
#include "Saver.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"

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

	Printer::safe_print("Saver: " + std::to_string(_currentFrameIndex));

	saveFrame();
}

void Saver::setSubject(FramesManager* subject)
{
	this->_subject = subject;
	_outputStream = new cv::VideoWriter("data/streaming/originalStream.avi",
 								 CV_FOURCC('X', '2', '6', '4'),
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

void Saver::saveFrame()
{
	*_outputStream << *_frame;
}
