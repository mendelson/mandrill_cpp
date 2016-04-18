#pragma once

// #include <memory>
// #include <opencv2/opencv.hpp>
// #include "FramesManager.hpp"
// #include "Observer.hpp"
#include "Processor.hpp"

class GreyProcessor : public Processor
{
public:
  void Update();

private:
  cv::Mat _greyFrame;
};
