#include <iostream>
#include "Observer.hpp"
#include "FramesManager.hpp"
#include "Analyser.hpp"

Analyser::Analyser()
{
	_subject = 0;
	// locked = false;
	_currentFrame = -1;
}

Analyser::~Analyser()
{
	_subject = 0;
}

void Analyser::Update(FramesManager* theChangedSubject)
{
	if(theChangedSubject == _subject)
	{
		showLastFrameIndex();
	}
}

void Analyser::setSubject(FramesManager* subject)
{
	_subject = subject;
}

void Analyser::showLastFrameIndex()
{
	auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
	_currentFrame = _subject->getLatestFrameIndex();
	// std::cout << "Analyser: " << _currentFrame << "|" << std::endl;
}