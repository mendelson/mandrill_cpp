#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "FramesManager.hpp"
#include "Observer.hpp"

class Processor : public Observer
{
public:
  virtual void Update();

private:
  void getCurrentFrame();

  std::shared_ptr<cv::Mat> _frame;

};
