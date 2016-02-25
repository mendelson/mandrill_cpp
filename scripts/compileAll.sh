#!/bin/bash

####################### DEPRECATED #######################
# Use makefile instead

mkdir -p ../bin

g++ -std=c++0x -pthread -I ../source/header ../source/src/main.cpp ../source/src/Camera.cpp ../source/src/FramesManager.cpp ../source/src/Analyser.cpp ../source/src/Printer.cpp `pkg-config --cflags opencv` -o ../bin/main `pkg-config --libs opencv` -O4

# g++ -std=c++0x ../src/shared_ptr_test.cpp `pkg-config --cflags opencv` -o ../bin/main `pkg-config --libs opencv` -O4