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
  _firstFrame = -1;
  movDetected = false;
  firstInBuffer = 0;

}

MovementProcessor::~MovementProcessor()
{
	_outputStream->release();
	delete _outputStream;
}

void MovementProcessor::Update()
{
	// bool objectDetected = false;
	cv::Mat thresholdImage;
	cv::Mat frame1, frame2;
	cv::Mat grayImage1,grayImage2;
	cv::Mat differenceImage;
	std::unique_lock<std::mutex> _lock(_mutex);

 	// verifies if we have frame to make
	// diff comparison
	if(_storedFrame.empty()){
  		getCurrentFrame();
  		_storedFrame = *_frame;
  		_storedFrameIndex = _currentFrameIndex;
	}

  	frame1 = _storedFrame;
  	std::cout << "[MOVE] index stored: " << _storedFrameIndex << std::endl;

	getCurrentFrame();

  	frame2 = *_frame;
  	std::cout << "[MOVE] current: " << _currentFrameIndex << std::endl;

  	//--------------------------------------
  	// these next few lines are for creating
  	// the frames with motion detection,
  	// the real motion deection is done in
  	// searchMovement, because the balck and white
  	// frame transformation to save can not be used
  	// as base to identify movement
  	//--------------------------------------
  	// converts received frames to gray scale
  	cv::cvtColor(frame1, grayImage1, CV_RGB2GRAY);
	cv::cvtColor(grayImage1, _greyFrame, CV_GRAY2RGB);
  	cv::cvtColor(frame2, grayImage2, CV_RGB2GRAY);
	cv::cvtColor(grayImage2, _greyFrame2, CV_GRAY2RGB);

	// verifies diff between frames in order to identify
	// movement
	cv::absdiff(_greyFrame, _greyFrame2, differenceImage);	
	// applies threshold on image
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	// applies blur in order to to reduce noise
	cv::blur(thresholdImage, thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	// binarize the frame again
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);


	// saves second frame as sotredFrame, in order ro be use
	// on the next call of the function, to detect if there are
	// movemente according to the next frame
	_storedFrame = frame2;
  	_storedFrameIndex = _currentFrameIndex;


  	// calls function that will identifie
  	// if movement occurred
  	searchMovement(frame1, frame2);

 //  	// if the buffer passes by MAXSAVING from the
 //  	// time the movement stopped, we save the frames 
 //  	// to a file, and clear movement flags
 //  // 	if(_lastFrame != -1 && _latestFrameIndex >= _lastFrame + MAXSAVING){
	// 	// saveBuffer();
	// 	// _lastFrame = -1;
	// 	// _firstFrame = -1;
	// 	// movDetected = false;

  // 	}

  	if(movDetected)
  		saveFrame(thresholdImage);



}

void MovementProcessor::setSubject(FramesManager* subject, unsigned int id)
{
	// gets current time
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream path;

	// gets  codecs and camera information
	this->_subject = subject;
	_id = id;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	// parametize saving path
	// ltm->tm_min
	//  ltm->tm_hour

	path <<  "data/streaming/moveStream/" << ltm->tm_mday << "." << extension;

	_outputStream = new cv::VideoWriter(path.str() ,
 								 fourccCode,
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));
}


void MovementProcessor::searchMovement(cv::Mat frame1, cv::Mat frame2)
{
	cv::Mat thresholdImage;
	cv::Mat temp;
	std::vector< std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat differenceImage;

	// Movement detection processing
  	// converts received frames to gray scale
	cv::cvtColor(frame1, _greyFrame, cv::COLOR_BGR2GRAY);
	cv::cvtColor(frame2, _greyFrame2, cv::COLOR_BGR2GRAY);
	// verifies diff between frames in order to identify
	// movement
	cv::absdiff(_greyFrame, _greyFrame2, differenceImage);	
	// applies threshold on image
	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);
	// applies blur in order to to reduce noise
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	// binarize the frame again
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	// verifies if detection ocurres by analysing diff treated image
	thresholdImage.copyTo(temp);
	// find edges
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );


	// verifies the occurrence of movement detection events
	switch (movDetected) {
		case true: // movement was detected in previous frame

			// movement is detected again
			if(contours.size() > 0){
					// Printer::safe_print("DETECTADO ANTES\n");
					movDetected = true;
					Printer::safe_print("Movement found\n");

			}
			// movement stopped
			else {
				movDetected = false;
				Printer::safe_print("Movement not found\n");
				_lastFrame = _latestFrameIndex;
			}
		break;

		case false: // movement was not detected till now
			// movement is detected for the first time

			if(contours.size() > 0){

					movDetected = true;
					Printer::safe_print("Movement found\n");
					_firstFrame = _latestFrameIndex;
					// videoLabel();
					_lastFrame = -1;

			// Printer::safe_print("NAO DETECTADO ANTES"+std::to_string(_firstFrame )+"\n");
			// Printer::safe_print("NAO DETECTADO ANTES"+std::to_string(_latestFrameIndex )+"\n");

			}
			// movement  still not detected
			else {
				movDetected = false;
				Printer::safe_print("Movement not found\n");
			}
		break;
	}
	

}



void MovementProcessor::videoLabel()
{
	// gets current time
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::stringstream path;

	// this->_subject = subject;
	std::string extension = CodecsConfig::getCodecExtension(_codecName);
	int fourccCode = CodecsConfig::getCodecFourcc(_codecName);

	if(extension.empty() || fourccCode == -4)
		exit(-4);

	path <<  "data/streaming/moveStream/" << ltm->tm_mday << 
	"_" << ltm->tm_min << "." << extension;

	_outputStream->release();
	_outputStream = new cv::VideoWriter(path.str() ,
 								 fourccCode,
 								 _subject->getCameraFPS(),
 								 cvSize((int)_subject->getFramesWidth(),(int)_subject->getFramesHeight()));

		Printer::safe_print("YOYO CARALHO:" + std::to_string((int)_subject->getFramesWidth()) + "\n");
	// exit(4);
}



void MovementProcessor::saveFrame(cv::Mat frame)
{
	// int a=2;
	*_outputStream << frame;

	// libera frame para poder ser deletado
	this->_subject->setFrameAsFree(_currentFrameIndex, _id);


}


void MovementProcessor::getCurrentFrame()
{
	_currentFrameIndex++;

	do{
			_frame = _subject->getFrame(_currentFrameIndex);
	}while(_frame ==  NULL);
}
