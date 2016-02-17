#include <iostream>
#include <memory>
#include "FramesManager.h"

FramesManager* FramesManager::_instance = 0;

FramesManager* FramesManager::Instance()
{
	if (_instance == 0)
	{
		_instance = new FramesManager();
	}

	return _instance;
}

FramesManager::FramesManager()
{
	latestFrame = -1;
}

void FramesManager::addFrame(cv::Mat frame)
{
	latestFrame++;
	
	// std::cout << latestFrame << std::endl;


	if(framesSet.size() < MAXFRAMES)
	{
		// std::shared_ptr<cv::Mat> newPointer;
		auto newPointer = std::make_shared<cv::Mat>(frame);
		// framesSet.emplace(latestFrame, /* shared_ptr para o cv::Mat frame */);
		
	}
	else
	{
		// framesSet[latestFrame - MAXFRAMES] = 0;
		framesSet.erase(latestFrame - MAXFRAMES);
		// auto it = framesSet.find(latestFrame - MAXFRAMES);

		// if(it == framesSet.end())
		// {
			// std::cout << "CRAP!!!!!" << std::endl;
		// }

		// it->first = latestFrame;
		// (it->second) = std::make_shared<cv::Mat>(frame);

		// framesSet[latestFrame - MAXFRAMES] = make_shared<cv::Mat>(frame);
	}

	framesSet.emplace(latestFrame, std::make_shared<cv::Mat>(frame));

	// std::cout << framesSet.size() << " | " << sizeof(framesSet) << std::endl;
	// std::cout << framesSet.size() << std::endl;

	// if(latestFrame > 500)
	// {
	// 	cv::imshow("Stored streaming", *framesSet[latestFrame - 500]);
	// }
}

cv::Mat FramesManager::getFrame(unsigned int index)
{
	return *framesSet[index];
}
