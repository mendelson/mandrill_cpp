#!/bin/bash

mkdir -p ../bin

g++ -std=c++0x -pthread ../src/main.cpp ../src/Camera.cpp ../src/CameraStandards.cpp  `pkg-config --cflags opencv` -o ../bin/main `pkg-config --libs opencv` -O4
