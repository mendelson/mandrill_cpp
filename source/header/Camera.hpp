#ifndef CAMERA_H
#define CAMERA_H

// #include <iostream>
// #include <opencv2/opencv.hpp>

class Camera
{
public:
    Camera(std::string url, std::string model);
    ~Camera();

    void updateFrame();
    cv::Mat getFrame();
    const std::string getIp();
    std::string getUrl();

private:
    const std::string _url;
    const std::string _model;
    std::string _ip;
    cv::VideoCapture* _frameCap;
    cv::Mat _frame;
};

#endif