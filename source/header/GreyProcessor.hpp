#pragma once

#include "Processor.hpp"

class GreyProcessor : public Processor
{
public:
  ~GreyProcessor();
  void Update();
  void setSubject(FramesManager* subject);

private:
  cv::Mat _greyFrame;
  cv::VideoWriter* _outputStream;
  void saveFrame(cv::Mat frame);
};
