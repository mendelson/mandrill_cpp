// #include <stdio.h>
#include <opencv2/opencv.hpp>
// #include <iostream>
#include "Camera.hpp"

Camera::Camera(std::string url, std::string model)
: _url(url),
  _model(model)
{
    // Extracting the IP from the URl
    char ip[30];
    std::size_t length;
    std::size_t begin = _url.find("@");
    if(begin != std::string::npos)
    {
        std::size_t end = _url.find("/", begin);

        length = _url.copy(ip, int (end - begin - 1), int (begin + 1));
    }
    else
    {
        begin = _url.find("://");

        std::size_t end = _url.find("/", begin + 3);

        length = _url.copy(ip, int (end - begin - 3), int (begin + 3));
    }

    ip[length] = '\0';
    _ip = ip;

    // Opening video capture
    _frameCap = new cv::VideoCapture(_url);

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

// .mp4 ou .mkv