#pragma once

#include <unordered_map>
#include "Processor.hpp"
#include <ctime>

typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;


class MovementProcessor : public Processor
{
public:
  MovementProcessor(std::string codecName);
  ~MovementProcessor();
  void Update();
  void searchMovement(cv::Mat, cv::Mat);
  void setSubject(FramesManager* subject);
  const static int SENSITIVITY_VALUE = 20;
  const static int BLUR_SIZE = 10;

private:
  cv::Mat _greyFrame;
  cv::Mat _greyFrame2;
  cv::Mat _storedFrame;
  cv::VideoWriter* _outputStream;
  unsigned int _storedFrameIndex;
  unsigned int _latestFrameIndex;
  unsigned int _lastFrame;
  unsigned int _firstFrame;
  std::string _codecName;
  FramesSet _cameraBuffer;
  void saveFrame(cv::Mat frame);
  void addFrame(cv::Mat frame);

};
