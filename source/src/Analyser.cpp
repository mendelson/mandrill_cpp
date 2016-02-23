#include <iostream>
#include "Observer.hpp"
#include "FramesManager.hpp"
#include "Analyser.hpp"

Analyser::Analyser()
{
	_subject = 0;
}

Analyser::~Analyser()
{
	_subject = 0;
}

void Analyser::Update(FramesManager* theChangedSubject)
{
	if(theChangedSubject == _subject)
	{
		showLastFrameIndex();
	}
}

void Analyser::setSubject(FramesManager* subject)
{
	_subject = subject;
}

void Analyser::showLastFrameIndex()
{
	auto frame = std::make_shared<cv::Mat>(_subject->getLatestFrame());
	std::cout << "New index received: " << _subject->getLatestFrameIndex() << std::endl;
}