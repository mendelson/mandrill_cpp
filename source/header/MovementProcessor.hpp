#pragma once

#include "Processor.hpp"

class MovementProcessor : public Processor
{
public:
  ~MovementProcessor();
  void Update();
  void searchMovement(cv::Mat, cv::Mat);
  void setSubject(FramesManager* subject);
  const static int SENSITIVITY_VALUE = 20;
  const static int BLUR_SIZE = 10;

private:
  cv::Mat _greyFrame;
  cv::Mat _greyFrame2;
  cv::VideoWriter* _outputStream;
  unsigned int _storedFrameIndex;
  cv::Mat _storedFrame;
  void saveFrame(cv::Mat frame);
};
