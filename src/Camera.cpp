#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "CameraStandards.h"
#include "Camera.h"

Camera::Camera(std::string protocol, std::string ip, std::string loginUser, std::string loginPassword, CameraStandards::MODEL model)
: _protocol(protocol),
  _ip(ip),
  _loginUser(loginUser),
  _loginPassword(loginPassword),
  _model(model),
  _snapShotPath(CameraStandards::getSnapShotPath(model)),
  _videoStreamPath(CameraStandards::getVideoStreamPath(model))
{}

void Camera::showSnapShot()
{
	cv::VideoCapture videoCapture;
    cv::Mat snapShot;

    const std::string snapShotAddress = _protocol + "://" +
    									_loginUser + ":" +
    									_loginPassword + "@" +
    									_ip + "/" +
    									_snapShotPath;

    
    if(!videoCapture.open(snapShotAddress))
    {
        std::cout << "Error acquiring snapshot" << std::endl;
        return;
    }

    if(!videoCapture.read(snapShot))
    {
        std::cout << "No frame" << std::endl;
        cv::waitKey();
    }

    cv::imshow("Snap Shot from " + _ip, snapShot);
}

void Camera::showVideoStreamRTSP()
{
    const std::string videoStreamAddress = _protocol + "://" +
                                        // _loginUser + ":" +
                                        // _loginPassword + "@" +
                                        _ip + "/" +
                                        _videoStreamPath;

    cv::Mat frame;
    cv::namedWindow("Live Streaming from " + _ip, 1);

    cv::VideoCapture cap(videoStreamAddress);
    
    while (cap.isOpened())
    {
        cap >> frame;
        if (frame.empty()) continue;

        cv::imshow("video", frame);

        if (cv::waitKey(30) >= 0) break;
    }
}