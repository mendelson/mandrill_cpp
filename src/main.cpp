#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <thread>
#include "Camera.h"
#include "FramesManager.h"

int main(int argc, char *argv[])
{
	FramesManager* manager = FramesManager::Instance();

	if(argc != 3)
	{
		std::cout << "It is mandatory to pass exactly the following parameters: URL and camera_model! Aborting..." << std::endl;

		return -1;
	}

	Camera* camera = new Camera(argv[1], argv[2]);

	unsigned int counter = -1;

	while(true)
	{
		camera->updateFrame();

		// cv::Mat frame = camera->getFrame();

		cv::Mat* frame = new cv::Mat(camera->getFrame().clone());


		if(!frame->empty())
		{
			counter++;

			cv::imshow("Live streaming from " + camera->getIp(), *frame);

			manager->addFrame(*frame);

			delete frame;

			// frame->release();

			// std::cout <<  << std::endl;
		}

		// if(counter > 5)
		// {
		// 	cv::imshow("Stored streaming", manager->getFrame(counter - 5));
		// }
		
		// "ESC" key aborts execution
		if (cv::waitKey(30) == 27) break;
	}
}