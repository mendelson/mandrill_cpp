#include <iostream>
#include "Saver.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include "CodecsConfig.hpp"

Saver::Saver(std::string codecName) : _codecName(codecName)
{
  _currentFrameIndex = -1;
  _subject = nullptr;
}

Saver::~Saver()
{
	_outputStream->release();
	delete _outputStream;
}

void Saver::Update()
{
	std::unique_lock<std::mutex> _lock(_mutex);
  	getCurrentFrame();

	Printer::safe_print("Saver: " + std::to_string(_currentFrameIndex));

	saveFrame();
}

void Saver::setSubject(FramesManager* subject)
{
	this->_subject = subject;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	std::string outputFile = "data/streaming/originalStream." + extension;
	_outputStream = new cv::VideoWriter(outputFile,
 								 fourccCode,
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

void Saver::saveFrame()
{
	*_outputStream << *_frame;
}
