#include <iostream>
#include "MovementProcessor.hpp"
#include "FramesManager.hpp"
#include "Printer.hpp"
#include "CodecsConfig.hpp"

extern bool movDetected;


MovementProcessor::MovementProcessor(std::string codecName) : _codecName(codecName)
{
  _currentFrameIndex = -1;
  _subject = nullptr;
  _latestFrameIndex = -1;

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

	addFrame(frame1);

  	// Printer::safe_print("MovementProcessor: " + std::to_string(_currentFrameIndex));

	while(_currentFrameIndex == _storedFrameIndex)
		getCurrentFrame();


  	// Printer::safe_print("MovementProcessor: " + std::to_string(_currentFrameIndex));

  	frame2 = *_frame;

	addFrame(frame2);

  	cv::cvtColor(frame2, grayImage2, CV_RGB2GRAY);
	cv::cvtColor(grayImage2, _greyFrame2, CV_GRAY2RGB);


	cv::absdiff(_greyFrame,_greyFrame2,differenceImage);	
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	_storedFrame = *_frame;
  	_storedFrameIndex = _currentFrameIndex;

  	searchMovement(frame1, frame2);

  	if(_currentFrameIndex >= _firstFrame && _currentFrameIndex <= _lastFrame)
		saveFrame(thresholdImage);

}

void MovementProcessor::setSubject(FramesManager* subject)
{
	// gets current time
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream path;

	this->_subject = subject;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	path <<  "data/streaming/moveStream/" << ltm->tm_mday << "." << extension;

	_outputStream = new cv::VideoWriter(path.str() ,
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
	cv::Mat thresholdImage;
	cv::Mat temp;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat differenceImage;

	// movement detection processing
	cv::cvtColor(frame1, _greyFrame, cv::COLOR_BGR2GRAY);
	cv::cvtColor(frame2, _greyFrame2, cv::COLOR_BGR2GRAY);
	cv::absdiff(_greyFrame, _greyFrame2, differenceImage);	
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	// verifies if detection ocurres by analysing diff treated image
	thresholdImage.copyTo(temp);
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	// verifies the occurrence of movement detection events
	switch (movDetected) {
		case true: // movement was detected in previous frame
			// movement is detected again
			if(contours.size() > 0){
					movDetected = true;
					Printer::safe_print("Movement found\n");
			}
			// movement stopped
			else {
				movDetected = false;
				Printer::safe_print("Movement not found\n");
				_lastFrame = _storedFrameIndex;
			}
		break;

		case false: // movement was not detected till now
			// movement is detected for the first time
			if(contours.size() > 0){
					movDetected = true;
					Printer::safe_print("Movement found\n");
					_firstFrame = _storedFrameIndex - 1;
					_lastFrame = -1;
			}
			// movement  still not detected
			else {
				movDetected = false;
				Printer::safe_print("Movement not found\n");
			}
		break;
	}
	

}


void MovementProcessor::addFrame(cv::Mat frame)
{

	if(_cameraBuffer.size() >= MAXFRAMES)
	{
		_cameraBuffer.erase(_latestFrameIndex + 1 - MAXFRAMES);
	}

	_cameraBuffer.emplace(_latestFrameIndex + 1, std::make_shared<cv::Mat>(frame));

	_latestFrameIndex++;
}