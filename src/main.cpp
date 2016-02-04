#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <thread>
#include "Camera.h"
#include "CameraStandards.h"

void vai(Camera* cam);

int main()
{

 	cv::Mat frame, frame1, frame2;
 	frame = cv::Mat::zeros(100, 100, CV_8U);
	std::vector<Camera*> camera;
	// std::vector<cv::VideoCapture*> frameCap;

	camera.push_back(new Camera("rtsp", "10.190.60.131", "root", "akts", CameraStandards::VIVOTEK_FD8136, frame));
	camera.push_back(new Camera("rtsp", "10.190.60.105", "root", "akts", CameraStandards::VIVOTEK_FD8136, frame1));
	camera.push_back(new Camera("rtsp", "10.190.60.125", "root", "akts", CameraStandards::AXIS_M1014, frame2));

	// std::cout << "1" << std::endl;

	// for(int i = 0; i < camera.size(); i++)
	// {
	// 	cv::namedWindow("Live streaming from " + camera.at(i)->getIp(), 1);
 //    	frameCap.push_back(new cv::VideoCapture(camera.at(i)->getVideoStreamAddress()));
	// }
	// std::cout << "2" << std::endl;

 //    cv::Mat frame;
 //    std::cout << "3" << std::endl;

	// while(true)frame
	// {
	// 	for(int i = 0; i < camera.size(); i++)
	// 	{
	// 		// std::cout << camera.at(i)->getVideoStreamAddress() << std::endl;

	// 		if(!frameCap.at(i)->isOpened()) break;

	// 		*(frameCap.at(i)) >> frame;

	// 		if (frame.empty()) continue;

	//         cv::imshow("Live streaming from " + camera.at(i)->getIp(), frame);

	//         if (cv::waitKey(30) >= 0) break;
	// 	}
	// }




	std::thread a(vai, camera.at(0));
	std::thread b(vai, camera.at(1));
	std::thread c(vai, camera.at(2));

	a.detach();
	b.detach();
	c.detach();

	// a.join();
	// b.join();
	// c.join();

	camera.clear();
	cv::waitKey(5000);
	while(1)
	{
		

		imshow("vidaidsof", frame);

		cv::waitKey(30);
	}
}

void vai(Camera* cam)
{
	cam->getVideoStreamAddress();
	cam->showVideoStreamRTSP();
}