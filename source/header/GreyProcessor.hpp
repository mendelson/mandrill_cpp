#pragma once

#include "Processor.hpp"

class GreyProcessor : public Processor
{
public:
	GreyProcessor(std::string codecName);
  	~GreyProcessor();
  	void Update();
  	void setSubject(FramesManager* subject, unsigned int id);

private:
  	cv::Mat _greyFrame;
  	cv::VideoWriter* _outputStream;
  	void saveFrame(cv::Mat frame);
  	std::string _codecName;
};
