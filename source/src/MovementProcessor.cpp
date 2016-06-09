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

	while(_currentFrameIndex == _storedFrameIndex)
		getCurrentFrame();


  	frame2 = *_frame;

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
	cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
	// binarize the frame again
	cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,cv::THRESH_BINARY);

	// saves second frame as sotredFrame, in order ro be use
	// on the next call of the function, to detect if there are
	// movemente according to the next frame
	_storedFrame = *_frame;
  	_storedFrameIndex = _currentFrameIndex;

  	// adds"~movement processed" image to buffer
  	addFrame(thresholdImage);

  	// calls function that will identifie
  	// if movement occurred
  	searchMovement(frame1, frame2);



  	// if the buffer passes by MAXSAVING from the
  	// time the movement stopped, we save the frames 
  	// to a file, and clear movement flags
  	if(_lastFrame != -1 && _latestFrameIndex >= _lastFrame + MAXSAVING){
		saveBuffer();
		_lastFrame = -1;
		_firstFrame = -1;
		movDetected = false;

  	}

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


void MovementProcessor::addFrame(cv::Mat frame)
{

	_mutex2.lock();
	// Printer::safe_print("ADICIONADO:" + std::to_string(_latestFrameIndex) + "\n");
	// Printer::safe_print("SIZE:" + std::to_string(_cameraBuffer.size()) + "\n");
	// Printer::safe_print("SIZEmax:" + std::to_string(MAXFRAMES) + "\n");

	if(_cameraBuffer.size() >= MAXFRAMES)
	{
		// if we have to erase buffer
		// but frames are in movement range
		// we have to save them in file, before
		// clearing the buffer
		if(_firstFrame != -1){
			Printer::safe_print("EU Q ADICIONEI\n");
			saveBuffer();
			Printer::safe_print("EU Q ADICIONEI\n");
			_firstFrame = _latestFrameIndex + 1;
			firstInBuffer = _latestFrameIndex + 1;
	  	}

		_cameraBuffer.erase(_latestFrameIndex + 1 - MAXFRAMES);

	}

	_cameraBuffer.emplace(_latestFrameIndex + 1, std::make_shared<cv::Mat>(frame));

	_latestFrameIndex++;

	_mutex2.unlock();

}


void MovementProcessor::saveBuffer()
{
	// int inicio = (_firstFrame - 10 >= 0) ? (_firstFrame - 10) :0;
	int inicio = 0;
	int i;

	// Printer::safe_print("YOYO AQUI:" + std::to_string(_firstFrame) + "\n");

	// if movement was detected
	if(_firstFrame != -1){

		if(firstInBuffer <= (_firstFrame - MAXSAVING))
			inicio = firstInBuffer;
		else
			inicio = _firstFrame;

		Printer::safe_print("YOYO INICIO:" + std::to_string(inicio) + "\n");
		Printer::safe_print("YOYO FIM:" + std::to_string(_latestFrameIndex) + "\n");

		for(i = inicio; i <= _latestFrameIndex; i++){
			*_outputStream << *_cameraBuffer[i];
		}
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