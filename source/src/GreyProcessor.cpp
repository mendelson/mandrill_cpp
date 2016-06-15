#include <iostream>
#include "GreyProcessor.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include "CodecsConfig.hpp"

GreyProcessor::GreyProcessor(std::string codecName) : _codecName(codecName)
{
  _currentFrameIndex = -1;
  _subject = nullptr;
}

GreyProcessor::~GreyProcessor()
{
	_outputStream->release();
	delete _outputStream;
}

void GreyProcessor::Update()
{
	std::unique_lock<std::mutex> _lock(_mutex);
  	getCurrentFrame();

	cv::Mat oneChannelGreyFrame;

	cv::cvtColor(*_frame, oneChannelGreyFrame, CV_RGB2GRAY);
	cv::cvtColor(oneChannelGreyFrame, _greyFrame, CV_GRAY2RGB);

	Printer::safe_print("GreyProcessor: " + std::to_string(_currentFrameIndex));

	saveFrame(_greyFrame);
}

void GreyProcessor::setSubject(FramesManager* subject, unsigned int id)
{
	this->_subject = subject;
	_id = id;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	std::string outputFile = "data/streaming/greyStream." + extension;
	_outputStream = new cv::VideoWriter(outputFile,
 								 fourccCode,
 								 _subject->FPS,
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

void GreyProcessor::saveFrame(cv::Mat frame)
{
	*_outputStream << frame;
	this->_subject->setFrameAsFree(_currentFrameIndex, _id);

}
