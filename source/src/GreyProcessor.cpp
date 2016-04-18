#include <iostream>
#include "GreyProcessor.hpp"

void GreyProcessor::Update()
{
  // auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());

	std::unique_lock<std::mutex> _lock(_mutex);
  getCurrentFrame();

	cv::cvtColor(*_frame, _greyFrame, CV_BGR2GRAY);

	std::cout << "GreyProcessor: " << _currentFrameIndex << "|" << std::endl;

  // TODO: video writer!
}
