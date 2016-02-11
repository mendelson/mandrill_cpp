#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <thread>
#include "Camera.h"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "It is mandatory to pass exactly the following parameters: URL and camera_model! Aborting..." << std::endl;

		return -1;
	}

	Camera* camera = new Camera(argv[1], argv[2]);

	while(true)
	{
		camera->updateFrame();

		cv::Mat frame = camera->getFrame();

		if(!frame.empty())
		{
			cv::imshow("Live streaming from " + camera->getIp(), camera->getFrame());
		}
		
		// "ESC" key aborts execution
		if (cv::waitKey(30) == 27) break;
	}
}