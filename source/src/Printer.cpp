#include <iostream>
#include "Observer.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"

Printer::Printer()
{
	_subject = 0;
	locked = false;
	_currentFrame = -1;
}

Printer::~Printer()
{
	_subject = 0;
}

void Printer::Update(FramesManager* theChangedSubject)
{
	if(theChangedSubject == _subject)
	{
		showLastFrameIndex();
	}
}

void Printer::setSubject(FramesManager* subject)
{
	_subject = subject;
}

void Printer::showLastFrameIndex()
{
	auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
	_currentFrame = _subject->getLatestFrameIndex();
	// std::cout << "Printer: " << _currentFrame << "|" << std::endl;
}