#pragma once

#include "Processor.hpp"
#include <ctime>
#include <sstream>
#include <string>

class Saver : public Processor
{
public:
	Saver(std::string codecName);
	~Saver();
	void Update();
	void setSubject(FramesManager *subject, unsigned int id);
	void setVideoName();

private:
	cv::VideoWriter *_outputStream;
	void saveFrame();
	std::string _codecName;
	unsigned int _frameCounter;
	unsigned int _fileCounter;
};

