#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "Observer.hpp"

class Processor : public Observer
{
public:
	virtual void Update() = 0;

protected:
	void getCurrentFrame();

  	std::shared_ptr<cv::Mat> _frame;
  	const std::string _codecName;
  	unsigned int _currentFrameIndex;
};
