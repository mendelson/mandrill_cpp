#pragma once

#include "Processor.hpp"

class Saver : public Processor
{
public:
  ~Saver();
  void Update();
  void setSubject(FramesManager* subject);

private:
  cv::VideoWriter* _outputStream;
  void saveFrame();
};
