#include "Processor.hpp"
#include "FramesManager.hpp"

void Processor::getCurrentFrame()
{
  _frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());
  _currentFrameIndex = _subject->getLatestFrameIndex();
}
