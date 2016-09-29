#include "MovementDetector.hpp"
#include <stdlib.h>
#include <string.h>

GMainLoop *MovementDetector::_loop = 0;
int MovementDetector::_movementCounter = 0;

MovementDetector::MovementDetector()
{
	_error = NULL;
	_movementCounter = 0;
}

bool MovementDetector::detectMovement(char *inputFile)
{
	_movementCounter = 0;
	_inputFile = inputFile;
	std::cout << "Aqui: " << _inputFile << std::endl;

  initPipeline();
  setPipelineInputFile();
  startPipeline();
  stopPipeline();

  return _movementCounter > 2 ? true : false;
}

gboolean MovementDetector::countMovementCallback (GstBus *bus, GstMessage *message, gpointer data)
{
  // std::cout << " | " << GST_MESSAGE_TYPE_NAME (message);
  // std::cout << " | " << GST_MESSAGE_TYPE (message);
	// GstMessageType m = GST_MESSAGE_TYPE (message);
  // std::cout << " | " << GST_MESSAGE_SRC (message);
  std::cout << " | " << gst_message_get_structure (message);

  // g_print ("Got %s message\n", GST_MESSAGE_TYPE_NAME (message));

  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR:
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      g_main_loop_quit (_loop);
      // return false;
      break;
    
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      g_main_loop_quit (_loop);
      // return false;
      break;
    
    /* Messages sent by motioncells */
    case 32768:
      // std::cout << "EH MOVIMENTO PRA CARAMBA!!!!!!!" << std::endl;
      _movementCounter++;
      // g_main_loop_quit (loop);
      break;
    
    /* unhandled message */
    default:
      break;
  }

  /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
  return true;
}

void MovementDetector::initPipeline()
{
  int s = 1;
  char **inputFileInit;

  inputFileInit = (char**) malloc(s);
  *inputFileInit = (char*) malloc(strlen(_inputFile));
  
  strcpy(*inputFileInit, _inputFile);

  gst_init (&s, &inputFileInit);

  // pipeline = gst_parse_launch("gst-launch-1.0 rtspsrc name=my_rtspsrc ! rtph264depay ! h264parse ! avdec_h264 ! video/x-raw ! videoconvert ! motioncells threshold=0.000000000001 sensitivity=0.98 motioncellthickness=-1 postallmotion=true gridx=32 gridy=32 name=movementDetector ! videoconvert ! ximagesink", &_error);
  _pipeline = gst_parse_launch("gst-launch-1.0 filesrc name=my_src ! decodebin ! video/x-raw ! videoconvert ! motioncells threshold=0.000000000001 sensitivity=0.98 motioncellthickness=-1 postallmotion=true gridx=32 gridy=32 name=movementDetector ! videoconvert ! ximagesink", &_error);
  // _pipeline = gst_parse_launch("gst-launch-1.0 filesrc name=my_src ! decodebin ! video/x-raw ! videoconvert ! motioncells threshold=0.000000000001 sensitivity=0.98 motioncellthickness=-1 postallmotion=true gridx=32 gridy=32 name=movementDetector ! fakesink", &_error);
  // _pipeline = gst_parse_launch("gst-launch-1.0 filesrc name=my_src ! decodebin ! video/x-raw ! videoconvert ! motioncells threshold=0 sensitivity=0 motioncellthickness=-1 postallmotion=true gridx=32 gridy=32 name=movementDetector ! videoconvert ! ximagesink", &_error);
  
  if(!_pipeline)
  {
    std::cout << "Error while launching pipeline" << std::endl;
  }

  free(*inputFileInit);
  free(inputFileInit);
}

void MovementDetector::setPipelineInputFile()
{
	_src = gst_bin_get_by_name (GST_BIN (_pipeline), "my_src");
  g_object_set (_src, "location", _inputFile, NULL);
}

void MovementDetector::startPipeline()
{
	// _motioncells = gst_bin_get_by_name (GST_BIN (_pipeline), "movementDetector");

  gst_element_set_state (_pipeline, GST_STATE_PAUSED);
  gst_element_set_state (_pipeline, GST_STATE_PLAYING);

  _bus = gst_element_get_bus (_pipeline);
  // _bus = gst_element_get_bus (_motioncells);

  _busWatchId = gst_bus_add_watch (_bus, countMovementCallback, NULL);
   
  _loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (_loop);
}

void MovementDetector::stopPipeline()
{
	// gst_bus_remove_watch (_bus);
	gst_element_set_state (_pipeline, GST_STATE_PAUSED);
  gst_element_set_state (_pipeline, GST_STATE_READY);
  gst_element_set_state (_pipeline, GST_STATE_NULL);
  gst_object_unref (_pipeline);
  g_source_remove (_busWatchId);
  g_main_loop_unref (_loop);
  g_object_unref (_src);
  // g_object_unref (_motioncells);
  gst_object_unref (_bus);

  _error = NULL;
  _inputFile = NULL;

  std::cout << "\n\n_movementCounter: " << _movementCounter << std::endl;
}