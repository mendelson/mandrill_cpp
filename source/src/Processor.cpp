#include "Processor.hpp"
#include <iostream>

void Processor::Update()
{
  // auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
  getCurrentFrame();
	// _currentFrame = _subject->getLatestFrameIndex();
	std::cout << "Processor: " << _currentFrame << "|" << std::endl;
  // std::cout << "ha!" << std::endl;
}

void Processor::getCurrentFrame()
{
  _frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());
  _currentFrame = _subject->getLatestFrameIndex();
}
