#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

const std::string OUTPUTROOT = "files";
const std::string TMPFOLDER  = "tmp";
const std::string DASHFOLDER = "dash";
const std::string SPLITTIME  = "2";  // Seconds

void setupEnvironment(std::string uuid);


int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Wrong number of input arguments!\n");
		exit(-1);
	}

	// Vão p uma classe
	std::string urlHighDef			  = argv[1];
	std::string urlLowDef			  = argv[2];
	std::string uuid				  = argv[3];
	int socket						  = atoi(argv[4]);
	std::string uuidPath			  = OUTPUTROOT + "/" + uuid;
	std::string tmpPath				  = uuidPath + "/" + TMPFOLDER;
	std::string dashPath			  = uuidPath + "/" + DASHFOLDER;
	std::string srcLocationParameters = "location=" + urlHighDef;
	std::string tmpLocationParameter  = "location=" + tmpPath + "/video%d.mp4";
	std::string maxSizeTime = "max-size-time=" + SPLITTIME + "000000000";
	/////////

	int counter = 0;
	std::stringstream ss;
	std::string str;
	std::string videoString;
	char MP4BoxPath[]	= "/usr/local/bin/MP4Box";
	char gstLaunchPath[] = "/usr/bin/gst-launch-1.0";

	setupEnvironment(uuid);

	// exit(-1);

	pid_t pid_inicial = fork(); /* Create a child process */

	// calls gstreamer
	switch(pid_inicial)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */
			/*
			execl(gstLaunchPath, gstLaunchPath, "-e", "rtspsrc",
					"location=rtsp://root:akts@10.190.60.102/live.sdp", "!",
					"rtph264depay", "!", "h264parse", "!", "splitmuxsink",
					"location=video%d.mp4", "max-size-time=2000000000",
					NULL);
			*/

			execl(gstLaunchPath, gstLaunchPath, "-e", "rtspsrc",
				  srcLocationParameter.c_str(), "!", "rtph264depay", "!",
				  "h264parse", "!", "splitmuxsink",
				  tmpLocationParameter.c_str(), maxSizeTime.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_inicial " << pid_inicial
					  << "\n";
			break;
	}

	sleep(10);
	// calls MP4BOX while gstreamers is still running
	while(true)
	{
		sleep(4);
		counter++;
		pid_t pid = fork(); /* Create a child process */

		switch(pid)
		{
			case -1: /* Error */
				std::cout << "Uh-Oh! fork() failed.\n";
				break;

			case 0: /* Child process */
				ss << counter;
				str			= ss.str();
				videoString = tmpPath + "/video" + str + ".mp4";
				std::cout << "String: " << videoString << "\n";

				execl(MP4BoxPath, MP4BoxPath, "-dash", "2000", "-profile",
					  "live", "-rap", "-dash-ctx",
					  (uuidPath + "/ctx.txt").c_str(), "-mpd-refresh", "2",
					  "-segment-name", "dash/dash_segment_", "-out",
					  (uuidPath + "/manifest.mpd").c_str(), videoString.c_str(),
					  NULL);

				break;

			default: /* Parent process */
				std::cout << "Process created with pid " << pid << "\n";
				int status;

				while(!WIFEXITED(status))
				{
					waitpid(pid, &status,
							0); /* Wait for the process to complete */
				}

				std::cout << "Process exited with " << WEXITSTATUS(status)
						  << "\n";

				break;
		}
	}

	return 0;
}

void setupEnvironment(std::string uuid)
{
	std::string uuidPath = OUTPUTROOT + "/" + uuid;
	std::string tmpPath  = uuidPath + "/" + TMPFOLDER;
	std::string dashPath = uuidPath + "/" + DASHFOLDER;
	struct stat st		 = {0};

	if(stat(OUTPUTROOT.c_str(), &st) == -1)
	{
		mkdir(OUTPUTROOT.c_str(), 0700);
	}

	if(stat(uuidPath.c_str(), &st) == -1)
	{
		mkdir(uuidPath.c_str(), 0700);
	}

	if(stat(tmpPath.c_str(), &st) == -1)
	{
		mkdir(tmpPath.c_str(), 0700);
	}

	if(stat(dashPath.c_str(), &st) == -1)
	{
		mkdir(dashPath.c_str(), 0700);
	}
}

