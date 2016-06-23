#pragma once

#include "Observer.hpp"
#include <memory>
#include <opencv2/opencv.hpp>

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

