#pragma once
#include <gst/gst.h>
#include <iostream>
#include <string.h>

class GstManager
{
public:
	GstManager(std::string urlH, std::string urlL, std::string uuid,
			   std::string path);
	void DebugPipeline(std::string file);
	int run();

private:
	int SetupElements(std::string urlH, std::string uuid, std::string path);
	int LinkElements();

	GstElement *pipeline, *rtspsrc, *tee;
	GstElement *decodeQueue, *decodebin, *videoconvert, *autovideosink;
	GstElement *fileQueue, *rtph264depay, *h264parse, *mp4mux, *filesink;
	GstPad *teeDecodePad, *teeFilePad;

	GMainLoop *mainLoop; /* GLib's Main Loop */
};

