#pragma once

#include "Processor.hpp"

class MeanCalculator : public Processor
{
public:
  void Update();

private:
  double _currentMean;
};
