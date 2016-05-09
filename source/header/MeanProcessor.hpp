#pragma once

#include "Processor.hpp"

class MeanProcessor : public Processor
{
public:
  ~MeanProcessor();
  void Update();
private:
  double _frame_mean;
  cv::VideoWriter* _outputStream;
};
