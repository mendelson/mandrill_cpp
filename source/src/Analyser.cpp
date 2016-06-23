#include "Analyser.hpp"
#include "FramesManager.hpp"
#include <iostream>

void Analyser::Update()
{
	showLastFrameIndex();
}

// void
// Analyser::setSubject(FramesManager*
// subject)
// {
// 	_subject = subject;
// }

void Analyser::showLastFrameIndex()
{
	auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
	_currentFrameIndex = _subject->getLatestFrameIndex();
	std::cout << "Analyser: " << _currentFrameIndex << "|" << std::endl;
	// std::cout << "Analyser" <<
	// std::endl;
}

