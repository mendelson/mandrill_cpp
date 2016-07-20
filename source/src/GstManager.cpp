#include "GstManager.hpp"
#include <gst/gst.h>
#include <gst/gstdebugutils.h>
#include <iostream>
#include <string>

/* This function is called when an error message is posted on the bus */
static void errorCallback(GstBus *bus, GstMessage *msg, GMainLoop *mainLoop)
{
	(void)bus;
	GError *err;
	gchar *debug_info;

	/* Print error details on the screen */
	gst_message_parse_error(msg, &err, &debug_info);
	g_printerr("Error received from element %s: %s\n",
			   GST_OBJECT_NAME(msg->src), err->message);
	g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
	g_clear_error(&err);
	g_free(debug_info);

	g_main_loop_quit(mainLoop);
}

GstManager::GstManager(std::string urlH, std::string urlL, std::string uuid,
					   std::string path)
{
	(void)urlL;
	/*
	  gst-launch-1.0 -e rtspsrc location=$urlH ! tee name=tp tp. ! queue !
	  decodebin ! videoconvert ! autovideosink tp. ! queue ! rtph264depay !
	  h264parse
	  ! mp4mux ! filesink location=$path/$uuid/test.mp4
	*/
	if(SetupElements(urlH, uuid, path))
		exit(1);
	if(LinkElements())
		exit(2);
}

int GstManager::SetupElements(std::string urlH, std::string uuid,
							  std::string path)
{

	/* Create the elements */
	rtspsrc = gst_element_factory_make("rtspsrc", "rtspsrc");
	tee		= gst_element_factory_make("tee", "tee");

	decodeQueue   = gst_element_factory_make("queue", "decodeQueue");
	decodebin	 = gst_element_factory_make("decodebin", "decodebin");
	videoconvert  = gst_element_factory_make("videoconvert", "videoconvert");
	autovideosink = gst_element_factory_make("autovideosink", "autovideosink");

	fileQueue	= gst_element_factory_make("queue", "fileQueue");
	rtph264depay = gst_element_factory_make("rtph264depay", "rtph264depay");
	h264parse	= gst_element_factory_make("h264parse", "h264parse");
	mp4mux		 = gst_element_factory_make("mp4mux", "mp4mux");
	filesink	 = gst_element_factory_make("filesink", "filesink");

	/* Create the empty pipeline */
	pipeline = gst_pipeline_new("test-pipeline");
	if(!pipeline || !rtspsrc || !tee || !decodeQueue || !decodebin ||
	   !videoconvert || !autovideosink || !fileQueue || !rtph264depay ||
	   !h264parse || !mp4mux || !filesink)
	{
		std::cout << "Not all elements could be created." << std::endl;
		return -1;
	}

	std::string location = path + "/" + uuid + "/" + "test.mp4";
	g_object_set(filesink, "location", location.c_str(), NULL);
	g_object_set(rtspsrc, "location", urlH.c_str(), NULL);

	return 0;
}

static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data)
{
	(void)bus;
	GMainLoop *loop = (GMainLoop *)data;

	switch(GST_MESSAGE_TYPE(msg))
	{

		case GST_MESSAGE_EOS:
			g_print("End of stream\n");
			g_main_loop_quit(loop);
			break;

		case GST_MESSAGE_ERROR:
		{
			gchar *debug;
			GError *error;

			gst_message_parse_error(msg, &error, &debug);
			g_free(debug);

			g_printerr("Error: %s\n", error->message);
			g_error_free(error);

			g_main_loop_quit(loop);
			break;
		}
		default:
			break;
	}

	return TRUE;
}


/* This function will be called by the pad-added signal */
static void pad_added_handler(GstElement *src, GstPad *new_pad, gpointer data)
{
	GstElement *sinkElem = (GstElement *)data;
	GstPad *sink_pad	 = gst_element_get_static_pad(sinkElem, "sink");
	GstPadLinkReturn ret;

	std::cout << "Received new pad '" << GST_PAD_NAME(new_pad) << "' from '"
			  << GST_ELEMENT_NAME(src) << "':" << std::endl;

	/* If our converter is already linked, we have nothing to do here */
	if(gst_pad_is_linked(sink_pad))
	{
		g_print("  We are already linked. Ignoring.\n");
		gst_object_unref(sink_pad);
		return;
	}


	/* Attempt the link */
	ret = gst_pad_link(new_pad, sink_pad);
	if(!GST_PAD_LINK_FAILED(ret))
		std::cout << "Link succeeded" << std::endl;
	else
		std::cout << "Link failed" << std::endl;

	/* Unreference the sink pad */
	gst_object_unref(sink_pad);
}


int GstManager::LinkElements()
{
	GstPadTemplate *tee_src_pad_template;
	GstPad *queueDecodePad, *queueFilePad;
	GstBus *bus;

	/* Link all elements that can be automatically linked because they have
	 * "Always" pads */
	gst_bin_add_many(GST_BIN(pipeline), rtspsrc, tee, decodeQueue, decodebin,
					 videoconvert, autovideosink, fileQueue, rtph264depay,
					 h264parse, mp4mux, filesink, NULL);

	if(!gst_element_link_many(fileQueue, rtph264depay, h264parse, mp4mux,
							  filesink, NULL))
	{
		std::cout << "Elements could not be linked.3" << std::endl;
		gst_object_unref(pipeline);
		return -1;
	}


	if(!gst_element_link(decodeQueue, decodebin))
	{
		std::cout << "Elements could not be linked.4" << std::endl;
		gst_object_unref(pipeline);
		return -1;
	}

	if(!gst_element_link(videoconvert, autovideosink))
	{
		std::cout << "Elements could not be linked.1" << std::endl;
		gst_object_unref(pipeline);
		return -1;
	}


	g_signal_connect(rtspsrc, "pad-added", G_CALLBACK(pad_added_handler), tee);
	g_signal_connect(decodebin, "pad-added", G_CALLBACK(pad_added_handler),
					 videoconvert);


	/* Manually link the Tee, which has "Request" pads */
	tee_src_pad_template = gst_element_class_get_pad_template(
		GST_ELEMENT_GET_CLASS(tee), "src_%u");

	teeDecodePad =
		gst_element_request_pad(tee, tee_src_pad_template, NULL, NULL);
	std::cout << "Obtained request pad" << gst_pad_get_name(teeDecodePad)
			  << "for decode branch." << std::endl;
	queueDecodePad = gst_element_get_static_pad(decodeQueue, "sink");

	teeFilePad = gst_element_request_pad(tee, tee_src_pad_template, NULL, NULL);
	std::cout << "Obtained request pad" << gst_pad_get_name(fileQueue)
			  << "for file branch." << std::endl;
	queueFilePad = gst_element_get_static_pad(fileQueue, "sink");

	if(gst_pad_link(teeDecodePad, queueDecodePad) != GST_PAD_LINK_OK ||
	   gst_pad_link(teeFilePad, queueFilePad) != GST_PAD_LINK_OK)
	{
		std::cerr << "Tee could not be linked" << std::endl;
		gst_object_unref(pipeline);
		return -1;
	}
	gst_object_unref(queueDecodePad);
	gst_object_unref(queueFilePad);

	/* Instruct the bus to emit signals for each received message, and connect
	 * to the interesting signals */
	bus = gst_element_get_bus(pipeline);
	gst_bus_add_signal_watch(bus);
	g_signal_connect(G_OBJECT(bus), "message::error", (GCallback)errorCallback,
					 mainLoop);
	gst_object_unref(bus);

	return 0;
}

void GstManager::DebugPipeline(const std::string file)
{
	GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL,
							  file.c_str());
}

int GstManager::run()
{


	/* Create a GLib Main Loop and set it to run */
	mainLoop = g_main_loop_new(NULL, FALSE);

	/* we add a message handler */
	GstBus *bus		   = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	guint bus_watch_id = gst_bus_add_watch(bus, bus_call, mainLoop);


	/* Start playing the pipeline */
	DebugPipeline("3");
	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	DebugPipeline("4");
	g_main_loop_run(mainLoop);

	/* Release the request pads from the Tee, and unref them */
	gst_element_release_request_pad(tee, teeDecodePad);
	gst_element_release_request_pad(tee, teeFilePad);
	gst_object_unref(teeDecodePad);
	gst_object_unref(teeFilePad);

	/* Free resources */
	gst_element_set_state(pipeline, GST_STATE_NULL);
	g_source_remove(bus_watch_id);
	gst_object_unref(pipeline);
	return 0;
}
