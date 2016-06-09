#pragma once

#include "Processor.hpp"

class Saver : public Processor
{
public:
	Saver(std::string codecName);
  	~Saver();
  	void Update();
  	void setSubject(FramesManager* subject, unsigned int id);

private:
  	cv::VideoWriter* _outputStream;
  	void saveFrame();
  	std::string _codecName;
};
