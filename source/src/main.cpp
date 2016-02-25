#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <thread>
#include "Camera.hpp"
#include "FramesManager.hpp"
#include "Analyser.hpp"
#include "Printer.hpp"

int main(int argc, char *argv[])
{
	FramesManager* manager = FramesManager::getManager();
	Analyser* anal = new Analyser();
	Printer* printer = new Printer();
	manager->Attach(anal);
	manager->Attach(printer);

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

		cv::Mat frame = camera->getFrame().clone();

		// cv::Mat* frame = new cv::Mat(camera->getFrame().clone());

		if(!frame.empty())
		{
			counter++;

			cv::imshow("Live streaming from " + camera->getIp(), frame);

			manager->addFrame(frame);
		}

		// "ESC" key aborts execution		
		char pressedKey = cv::waitKey(30);
		// std::cout << (int) pressedKey << std::endl;
		if (pressedKey == 27) break;
	}
}