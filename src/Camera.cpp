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

void Camera::getSnapShot()
{
	cv::VideoCapture videoCapture;
    cv::Mat snapShot;

    const std::string snapShotAddress = _protocol + "://" +
    									_loginUser + ":" +
    									_loginPassword + "@" +
    									_ip + "/" +
    									_snapShotPath;

    for(;;) {
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

        cv::imshow("Snap Shot", snapShot);
        // std::cout << videoCapture.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;

        if(cv::waitKey(1) >= 0) break;
    }   
}

// void Camera::showVideoStream()
// {
// 	cv::VideoCapture vcap;
//     cv::Mat frame;

//     const std::string videoStreamAddress = _protocol + "://" +
//     									   _loginUser + ":" +
//     									   _loginPassword + "@" +
//     									   _ip + "/" +
//     									   _videoStreamPath;

//     std::cout << videoStreamAddress << std::endl;
    
//     //open the video stream and make sure it's opened
//     if(!vcap.open(videoStreamAddress))
//     {
//         std::cout << "Error opening video stream or file" << std::endl;
//         return;
//     }

//     for(;;)
//     {
//         if(!vcap.read(frame)) {
//             std::cout << "No frame" << std::endl;
//             cv::waitKey();
//         }

//         cv::imshow("Snap Shot", frame);

//         if(cv::waitKey(1) >= 0) break;
//     }   
// }

void Camera::showVideoStream()
{
    cv::VideoCapture vcap;
    cv::Mat frame;

    const std::string videoStreamAddress = _protocol + "://" +
                                           _loginUser + ":" +
                                           _loginPassword + "@" +
                                           _ip + "/" +
                                           _videoStreamPath;

    std::cout << videoStreamAddress << std::endl;
    
    //open the video stream and make sure it's opened
    if(!vcap.open(videoStreamAddress))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return;
    }

    for(;;)
    {
        if(!vcap.read(frame)) {
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }

        cv::imshow("Snap Shot", frame);

        if(cv::waitKey(1) >= 0) break;
    }   
}