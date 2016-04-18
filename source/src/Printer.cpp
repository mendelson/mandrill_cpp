#include <iostream>
#include "Observer.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"

void Printer::Update()
{
	showLastFrameIndex();
}

// void Printer::setSubject(FramesManager* subject)
// {
// 	_subject = subject;
// }

void Printer::showLastFrameIndex()
{
	auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
	_currentFrameIndex = _subject->getLatestFrameIndex();
	std::cout << "Printer: " << _currentFrameIndex << "|" << std::endl;
	// std::cout << "Printer" << std::endl;
}
