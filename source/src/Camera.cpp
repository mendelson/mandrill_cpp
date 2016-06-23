// #include <stdio.h>
#include <opencv2/opencv.hpp>
// #include <iostream>
#include "Camera.hpp"

Camera::Camera(std::string url, std::string model) : _url(url), _model(model)
{
	// Extracting the IP from the URl
	char ip[30];
	std::size_t length;
	std::size_t begin = _url.find("@");
	if(begin != std::string::npos)
	{
		std::size_t end = _url.find("/", begin);

		length = _url.copy(ip, int(end - begin - 1), int(begin + 1));
	}
	else
	{
		begin = _url.find("://");

		std::size_t end = _url.find("/", begin + 3);

		length = _url.copy(ip, int(end - begin - 3), int(begin + 3));
	}

	ip[length] = '\0';
	_ip		   = ip;

	// Opening video capture
	std::cout << "Trying to establish "
				 "connection to "
			  << _ip << std::endl;
	connect();

	// ///////////////////////////////////
	// int ex =
	// static_cast<int>(_frameCap->get(CV_CAP_PROP_FOURCC));
	//
	// // Transform from int to char via
	// Bitwise operators
	// char EXT[] = {(char)(ex &
	// 0XFF),(char)((ex & 0XFF00) >>
	// 8),(char)((ex &
	// 0XFF0000) >> 16),(char)((ex &
	// 0XFF000000) >> 24),0};
	// std::cout << "Olha: |" << EXT <<
	// "|" << std::endl;
	// // std::cout << "Olha: " <<
	// _frameCap->get(CV_CAP_PROP_FOURCC)
	// <<
	// std::endl;
	// ///////////////////////////////////
}

void Camera::updateFrame()
{
	if(_frameCap->isOpened())
	{
		*_frameCap >> _frame;
	}
}

cv::Mat Camera::getFrame()
{
	return _frame;
}

const std::string Camera::getIp()
{
	return _ip;
}

std::string Camera::getUrl()
{
	return _url;
}

Camera::~Camera()
{
	free(_frameCap);
}

double Camera::getWidth()
{
	return _width;
}

double Camera::getHeight()
{
	return _height;
}

double Camera::getFps()
{
	_fps = _frameCap->get(CV_CAP_PROP_FPS);
	return _fps;
}

// void Camera::reconnect()
// {
// free(_frameCap);
// connect();
// std::cout << "Connection
// reestablished! " <<
// _frameCap->get(CV_CAP_PROP_FPS)
// << std::endl;
// }

void Camera::connect()
{
	// bool connected = false;

	// while(!connected)
	// {
	// connected = true;

	// try
	// {
	// if(_frameCap == NULL)
	_frameCap = new cv::VideoCapture(_url);
	// else
	// _frameCap->open(_url);
	_width  = _frameCap->get(CV_CAP_PROP_FRAME_WIDTH);
	_height = _frameCap->get(CV_CAP_PROP_FRAME_HEIGHT);
	_fps	= _frameCap->get(CV_CAP_PROP_FPS);
	// }
	// catch(cv::Exception& e)
	// {
	//     std::cout << "\nCould not
	//     establish connection to " <<
	//     _ip <<
	//     "\nRetrying..." << std::endl;
	//     connected = false;
	// }
	// }
	// std::cout <<
	// _frameCap->isOpened() <<
	// std::endl;
}

// .mp4 ou .mkv

