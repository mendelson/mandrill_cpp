#include <iostream>
#include "MovementProcessor.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include "CodecsConfig.hpp"

MovementProcessor::MovementProcessor(std::string codecName) : _codecName(codecName)
{
  _currentFrameIndex = -1;
  _subject = nullptr;
}

MovementProcessor::~MovementProcessor()
{
	_outputStream->release();
	delete _outputStream;
	// std::cout << "wow!" << std::endl;
}

void MovementProcessor::Update()
{
	// bool objectDetected = false;
	cv::Mat frame1, frame2;
	cv::Mat grayImage1,grayImage2;
	cv::Mat differenceImage;
	cv::Mat thresholdImage;
	std::unique_lock<std::mutex> _lock(_mutex);


	if(_storedFrame.empty()){
  		getCurrentFrame();
  		_storedFrame = *_frame;
  		_storedFrameIndex = _currentFrameIndex;
	}

  	frame1 = _storedFrame;

  	cv::cvtColor(frame1, grayImage1, CV_RGB2GRAY);
	cv::cvtColor(grayImage1, _greyFrame, CV_GRAY2RGB);


  	// Printer::safe_print("MovementProcessor: " + std::to_string(_currentFrameIndex));

	while(_currentFrameIndex == _storedFrameIndex)
		getCurrentFrame();


  	// Printer::safe_print("MovementProcessor: " + std::to_string(_currentFrameIndex));

  	frame2 = *_frame;

  	cv::cvtColor(frame2, grayImage2, CV_RGB2GRAY);
	cv::cvtColor(grayImage2, _greyFrame2, CV_GRAY2RGB);


	cv::absdiff(_greyFrame,_greyFrame2,differenceImage);	
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	_storedFrame = *_frame;
  	_storedFrameIndex = _currentFrameIndex;

	saveFrame(thresholdImage);

	searchMovement(frame1, frame2);
}

void MovementProcessor::setSubject(FramesManager* subject)
{
	this->_subject = subject;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	_outputStream = new cv::VideoWriter("data/streaming/moveStream." + extension,
 								 fourccCode,
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}

// void MovementProcessor::saveFrame(cv::Mat frame, cv::VideoWriter* outputStream)
void MovementProcessor::saveFrame(cv::Mat frame)
{
	*_outputStream << frame;
}


void MovementProcessor::searchMovement(cv::Mat frame1, cv::Mat frame2)
{
	bool movDetected = false;
	cv::Mat thresholdImage;
	cv::Mat temp;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat differenceImage;


	cv::cvtColor(frame1, _greyFrame, cv::COLOR_BGR2GRAY);
	cv::cvtColor(frame2, _greyFrame2, cv::COLOR_BGR2GRAY);
	cv::absdiff(_greyFrame, _greyFrame2, differenceImage);	
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	thresholdImage.copyTo(temp);

	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	if(contours.size() > 0){
		movDetected = true;
		Printer::safe_print("Movement found\n");

	}
	else {
		movDetected = false;
		Printer::safe_print("Movement not found\n");

	}



}
