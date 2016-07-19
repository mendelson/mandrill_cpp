#include "Processor.hpp"
#include "FramesManager.hpp"

void Processor::getCurrentFrame()
{
	_currentFrameIndex++;

	_frame = _subject->getFrame(_currentFrameIndex);
	if(_frame == NULL)
		std::cout << "NULL\n";
}
