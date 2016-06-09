#include "Observer.hpp"

Observer::Observer()
{
  _currentFrameIndex = -1;
  _subject = nullptr;
}

void Observer::setSubject(FramesManager* subject, unsigned int id)
{
	this->_subject = subject;
	_id = id;
}
