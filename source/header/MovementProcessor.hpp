#pragma once

#include <unordered_map>
#include "Processor.hpp"
#include <ctime>

const int MAXSAVING = 10;

typedef std::unordered_map<unsigned int, std::shared_ptr<cv::Mat>> FramesSet;


class MovementProcessor : public Processor
{
public:
  MovementProcessor(std::string codecName);
  ~MovementProcessor();
  void Update();
  void searchMovement(cv::Mat, cv::Mat);
  void setSubject(FramesManager* subject, unsigned int id);
  void saveFrame(cv::Mat);
  void getCurrentFrame();
  const static int SENSITIVITY_VALUE = 20;
  const static int BLUR_SIZE = 10;

private:
  cv::Mat _greyFrame;
  cv::Mat _greyFrame2;
  cv::Mat _storedFrame;
  cv::VideoWriter* _outputStream;
  unsigned int _storedFrameIndex;
  unsigned int firstInBuffer;
  int _latestFrameIndex;
  int _lastFrame;
  int _firstFrame;
  std::string _codecName;
  FramesSet _cameraBuffer;
  void addFrame(cv::Mat frame);
  void saveBuffer();
  void videoLabel();

  std::mutex _mutex2;

};
