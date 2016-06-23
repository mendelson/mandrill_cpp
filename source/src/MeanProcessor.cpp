#include "MeanProcessor.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include <iostream>

MeanProcessor::~MeanProcessor()
{
	_outputStream->release();
	delete _outputStream;
}

void MeanProcessor::Update()
{
	// auto frame =
	// std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
	getCurrentFrame();

	// cv::Scalar tempVal =
	// cv::mean(*_frame);

	// Printer::safe_print("[MeanProcessor]
	// Frame[" +
	// std::to_string(_currentFrameIndex)
	// +
	// 	"] has mean of :"+
	// std::to_string(tempVal.val[0]));

	// this->_subject->setFrameAsFree(_currentFrameIndex,
	// _id);
}

