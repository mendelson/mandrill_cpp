#!/bin/bash

mkdir -p ../bin

g++ -std=c++0x -pthread ../src/main.cpp ../src/Camera.cpp ../src/CameraStandards.cpp ../src/FramesManager.cpp ../src/Analyser.cpp `pkg-config --cflags opencv` -o ../bin/main `pkg-config --libs opencv` -O4

# g++ -std=c++0x ../src/shared_ptr_test.cpp `pkg-config --cflags opencv` -o ../bin/main `pkg-config --libs opencv` -O4