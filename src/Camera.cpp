#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "CameraStandards.h"
#include "Camera.h"

Camera::Camera(std::string protocol, std::string ip, std::string loginUser, std::string loginPassword, CameraStandards::MODEL model, cv::Mat frame)
: _protocol(protocol),
  _ip(ip),
  _loginUser(loginUser),
  _loginPassword(loginPassword),
  _model(model),
  _snapShotPath(CameraStandards::getSnapShotPath(model)),
  _frame(frame)
{
    _videoStreamAddress = _protocol + "://" +
                                      _loginUser + ":" +
                                      _loginPassword + "@" +
                                      _ip + "/" +
                                      CameraStandards::getVideoStreamPath(model);
}

void Camera::showSnapShot()
{
	cv::VideoCapture videoCapture;
    cv::Mat snapShot;

    const std::string snapShotAddress = "http://" +
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
    // cv::Mat frame;
    // cv::namedWindow("Live streaming from " + _ip, 1);

    cv::VideoCapture frameCap(_videoStreamAddress);

    while (frameCap.isOpened())
    {
        frameCap >> _frame;
        if (_frame.empty()) continue;

        //cv::imshow("Live streaming from " + _ip, _frame);

        if (cv::waitKey(30) >= 0) break;
    }

    // cv::Mat frame;
    // cv::namedWindow("Live streaming from " + _ip, 1);

    // cv::VideoCapture frameCap(videoStreamAddress);

    // while (frameCap.isOpened())
    // {
    //     frameCap >> frame;
    //     if (frame.empty()) continue;

    //     cv::imshow("Live streaming from " + _ip, frame);

    //     if (cv::waitKey(30) >= 0) break;
    // }
/////////////////////////////////////////////////////////////////////////////////////
    // const std::string videoStreamAddress = "rtsp://10.190.60.117/live.sdp";

    // cv::Mat frame;
    // cv::namedWindow("Live streaming from 1", 1);

    // cv::VideoCapture frameCap(videoStreamAddress);

    // /////////////////////////////////////////////////////////////////////////////////////
    // const std::string videoStreamAddress1 = "rtsp://10.190.60.109/live.sdp";

    // cv::Mat frame1;
    // cv::namedWindow("Live streaming from 2", 1);

    // cv::VideoCapture frameCap1(videoStreamAddress1);

    // /////////////////////////////////////////////////////////////////////////////////////
    // const std::string videoStreamAddress2 = "rtsp://root:akts@10.190.60.118/axis-media/media.amp";

    // cv::Mat frame2;
    // cv::namedWindow("Live streaming from 3", 1);

    // cv::VideoCapture frameCap2(videoStreamAddress2);
    
    // while (frameCap.isOpened())
    // {
    //     frameCap >> frame;
    //     if (frame.empty()) continue;

    //     cv::imshow("Live streaming from 1", frame);

    //     if (cv::waitKey(30) >= 0) break;

    //     frameCap1 >> frame1;
    //     if (frame1.empty()) continue;

    //     cv::imshow("Live streaming from 2", frame1);

    //     if (cv::waitKey(30) >= 0) break;

    //     frameCap2 >> frame2;
    //     if (frame2.empty()) continue;

    //     cv::imshow("Live streaming from 3", frame2);

    //     if (cv::waitKey(30) >= 0) break;
    // }
}

const std::string Camera::getIp()
{
    return _ip;
}

std::string Camera::getVideoStreamAddress()
{
    std::cout << "-" + _videoStreamAddress + "-" << std::endl;
    return _videoStreamAddress;
}

// .mp4 ou .mkv