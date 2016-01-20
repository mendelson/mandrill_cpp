#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include "CameraStandards.h"

class Camera
{
public:
    Camera(std::string _protocol, std::string ip, std::string _loginUser, std::string _loginPassword, CameraStandards::MODEL model);

    void getSnapShot();
    void showVideoStream();

private:
    const std::string _ip;
    const std::string _loginUser;
    const std::string _loginPassword;
    const std::string _protocol;
    const std::string _snapShotPath;
    const std::string _videoStreamPath;
    const CameraStandards::MODEL _model;
};

#endif