#include "Saver.hpp"
#include "CodecsConfig.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include <iostream>

Saver::Saver(std::string codecName) : _codecName(codecName)
{
	_currentFrameIndex = -1;
	_subject		   = nullptr;
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

	Printer::safe_print("Saver: " + std::to_string(_currentFrameIndex) + "\n");

	saveFrame();
}

void Saver::setSubject(FramesManager *subject, unsigned int id)
{
	this->_subject		  = subject;
	_id					  = id;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode		  = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	std::string outputFile = "data/streaming/" + this->_subject->getModel() +
							 "_originalStream." + extension;
	_outputStream =
		new cv::VideoWriter(outputFile, fourccCode, _subject->FPS,
							cvSize((int)_subject->getFramesWidth(),
								   (int)_subject->getFramesHeight()));
}

void Saver::saveFrame()
{
	// int a=2;

	if(_frame != NULL)
		*_outputStream << *_frame;

	// libera frame para poder ser
	// deletado
	this->_subject->setFrameAsFree(_currentFrameIndex, _id);

	// a =
	// this->_subject->getFrameAvailability(_currentFrameIndex);

	// std::cout << "olha o aaaaaa: " <<
	// _currentFrameIndex << std::endl;
}

