#include "MovementDetector.hpp"
#include <sstream>
#include <stdlib.h>
#include <string.h>

GMainLoop *MovementDetector::_loop	 = 0;
int MovementDetector::_movementCounter = 0;
const std::string MOTIONCELLSMASK	  = "movementDetector";
const std::string PIPELINEPARTONE	  = "gst-launch-1.0 filesrc name=my_src ! "
									"decodebin ! video/x-raw ! videoconvert ! "
									"motioncells";
const std::string PIPELINEPARTTWO = " ! videoconvert ! fakesink";
// const std::string PIPELINEPARTTWO = " ! videoconvert ! ximagesink";

MovementDetector::MovementDetector()
{
	_error			 = NULL;
	_movementCounter = 0;
}

bool MovementDetector::detectMovement(char *inputFile)
{
	_movementCounter = 0;
	_inputFile		 = inputFile;

	// _sensitivity = 0;
	_sensitivity = 0.98;
	// _threshold = 1;
	// _threshold = 0.004;
	_threshold = 0.000000000001;
	std::ostringstream sSensitivity;
	std::ostringstream sThreshold;
	sSensitivity << _sensitivity;
	sThreshold << _threshold;
	_pipelineDescription =
		PIPELINEPARTONE + " sensitivity=" + sSensitivity.str() + " threshold=" +
		sThreshold.str() +
		" motioncellthickness=-1 postallmotion=true gridx=32 gridy=32 name=" +
		MOTIONCELLSMASK + PIPELINEPARTTWO;
	// _pipelineDescription = PIPELINEPARTONE + " sensitivity=" +
	// sSensitivity.str() + " threshold=" + sThreshold.str() + "
	// motioncellthickness=-1 postallmotion=false gridx=32 gridy=32 name=" +
	// MOTIONCELLSMASK + PIPELINEPARTTWO;

	// std::cout << "Arquivo em analise: " << _inputFile << std::endl;

	initPipeline();
	setPipelineInputFile();
	startPipeline();
	stopPipeline();

	return _movementCounter;
}

gboolean MovementDetector::countMovementCallback(GstBus *bus,
												 GstMessage *message,
												 gpointer data)
{
	// std::cout << " | " << GST_MESSAGE_TYPE_NAME (message);
	// std::cout << " | " << GST_MESSAGE_TYPE (message);
	// GstMessageType m = GST_MESSAGE_TYPE (message);
	// std::cout << " | " << GST_MESSAGE_SRC (message);
	// std::cout << " | " << gst_message_get_structure (message);

	// std::cout << " | " << GST_MESSAGE_SRC_NAME(message);
	// std::string teste = "movementDetector";

	switch(GST_MESSAGE_TYPE(message))
	{
		case GST_MESSAGE_ERROR:
			GError *err;
			gchar *debug;

			gst_message_parse_error(message, &err, &debug);
			g_print("\nError: %s\n", err->message);
			g_error_free(err);
			g_free(debug);

			g_main_loop_quit(_loop);
			// return false;
			break;

		case GST_MESSAGE_EOS:
			/* end-of-stream */
			g_main_loop_quit(_loop);
			// return false;
			break;

		/* Messages sent by motioncells */
		case GST_MESSAGE_ELEMENT:

			// std::cout << "EH MOVIMENTO PRA CARAMBA!!!!!!!" << std::endl;
			if(GST_MESSAGE_SRC_NAME(message) == MOTIONCELLSMASK)
			{
				// std::cout << "\nMensagem de interesse enviada por " <<
				// GST_MESSAGE_SRC_NAME(message) << " de tipo "
				// GST_MESSAGE_TYPE_NAME(message) << std::endl;
				_movementCounter++;
			}
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

	inputFileInit  = (char **)malloc(s);
	*inputFileInit = (char *)malloc(strlen(_inputFile));

	strcpy(*inputFileInit, _inputFile);

	gst_init(&s, &inputFileInit);

	_pipeline = gst_parse_launch(_pipelineDescription.c_str(), &_error);

	if(!_pipeline)
	{
		std::cout << "Error while launching pipeline. Aborting..." << std::endl;
		exit(-1);
	}

	free(*inputFileInit);
	free(inputFileInit);
}

void MovementDetector::setPipelineInputFile()
{
	_src = gst_bin_get_by_name(GST_BIN(_pipeline), "my_src");
	g_object_set(_src, "location", _inputFile, NULL);
}

void MovementDetector::startPipeline()
{
	gst_element_set_state(_pipeline, GST_STATE_PAUSED);
	gst_element_set_state(_pipeline, GST_STATE_PLAYING);

	_bus = gst_element_get_bus(_pipeline);

	_busWatchId = gst_bus_add_watch(_bus, countMovementCallback, NULL);

	_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(_loop);
}

void MovementDetector::stopPipeline()
{
	gst_element_set_state(_pipeline, GST_STATE_PAUSED);
	gst_element_set_state(_pipeline, GST_STATE_READY);
	gst_element_set_state(_pipeline, GST_STATE_NULL);
	gst_object_unref(_pipeline);
	g_source_remove(_busWatchId);
	g_main_loop_unref(_loop);
	g_object_unref(_src);
	gst_object_unref(_bus);

	_error	 = NULL;
	_inputFile = NULL;

	std::cout << "\n\n_movementCounter: " << _movementCounter << std::endl;
}

