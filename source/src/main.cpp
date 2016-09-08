#include <iostream>
#include <limits.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string>
#include <sys/inotify.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

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
	std::string urlHighDef			 = argv[1];
	std::string urlLowDef			 = argv[2];
	std::string uuid				 = argv[3];
	int socket						 = atoi(argv[4]);
	std::string uuidPath			 = OUTPUTROOT + "/" + uuid;
	std::string tmpPath				 = uuidPath + "/" + TMPFOLDER;
	std::string dashPath			 = uuidPath + "/" + DASHFOLDER;
	std::string srcLocationParameter = "location=" + urlHighDef;
	std::string tmpLocationParameter = "location=" + tmpPath + "/video%d.mp4";
	std::string maxSizeTime = "max-size-time=" + SPLITTIME + "000000000";

	std::cout << "Socket port: " << socket << std::endl;
	/////////

	int counter = 0;
	char buf[BUF_LEN];
	std::stringstream ss;
	std::string str;
	std::string videoString;
	char MP4BoxPath[]	= "/usr/local/bin/MP4Box";
	char gstLaunchPath[] = "/usr/bin/gst-launch-1.0";

	setupEnvironment(uuid);

	pid_t pid_gst = fork(); /* Create a child process */

	// gst-launch-1.0 -e rtspsrc
	// location=rtsp://root:akts@10.190.60.102/live.sdp ! rtph264depay !
	// h264parse ! splitmuxsink location=./video%d.mp4 max-size-time=2000000000

	// calls gstreamer
	switch(pid_gst)
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
			std::cout << "Process created with pid_gst " << pid_gst
					  << "\n";
			break;
	}


	int inotifyFd = inotify_init();
	if(inotifyFd == -1)
	{
		std::cout << "inotify_init failed" << std::endl;
		exit(1);
	}

	if(inotify_add_watch(inotifyFd, tmpPath.c_str(), IN_CLOSE_WRITE) == -1)
	{
		std::cout << "inotify_add_watch failed" << std::endl;
		exit(2);
	}
	int status;
	struct pollfd ufds[2];

	ufds[0].fd	 = inotifyFd;
	ufds[0].events = POLLIN | POLLPRI;


	// calls MP4BOX while gstreamers is still running
	while(true)
	{

		pid_t gst_status = waitpid(pid_gst, &status, WNOHANG);

		if(gst_status != 0)
		{
			std::cout << "==========\nDEU RUIM NO GSTREAMER!!! OLHA A TRETA "
				"EXPLODINDO!!!!!\n=========="
				<< std::endl;
			exit(-1);
		}

		int polln = poll(ufds, 1, 5000);  // poll com timeout de 5s

		if(polln == -1)
		{
			perror("poll");
			exit(-1);
		}
		else if(polln == 0)
		{
			std::cout << "timeout" << std::endl;
		}
		//		std::cout << "==========\nPID AQUI: " << pid_gst <<
		// std::endl;

		counter++;

		int numRead = read(inotifyFd, buf, BUF_LEN);

		if (numRead <= 0)
		{
			std::cout << "read failed" << std::endl;
			exit(numRead);
		}

		for (char *p = buf; p < buf + numRead; )
		{
			struct inotify_event *event = (struct inotify_event *)p;
			std::cout << "dash: " << event->name << std::endl;
			/* TODO: filtrar entrada, checar padrao: "video%d.mp4" */
			//callMP4Box(event->name);
			pid_t pid = fork(); /* Create a child process */

			switch(pid)
			{
				case -1: /* Error */
					std::cout << "Uh-Oh! fork() failed.\n";
					break;

				case 0: /* Child process */
					ss << counter;
					str			= ss.str();
					videoString = tmpPath + "/" + event->name;
					std::cout << "String: " << videoString << "\n";

					execl(MP4BoxPath, MP4BoxPath, "-dash", "2000", "-profile",
							"live", "-rap", "-dash-ctx",
							(uuidPath + "/ctx.txt").c_str(), "-mpd-refresh", "60",
							"-time-shift", "1800", "-min-buffer", "0",
							"-segment-name", "dash/dash_segment_", "-out",
							(uuidPath + "/manifest.mpd").c_str(), videoString.c_str(),
							"-dynamic", NULL);

					break;

				default: /* Parent process */
					std::cout << "Process created with pid " << pid << "\n";
					int status;

					while(!WIFEXITED(status))
					{
						/* Wait for the process to complete */
						waitpid(pid, &status, 0);
					}

					std::cout << "Process exited with " << WEXITSTATUS(status)
						<< "\n";

					break;
			}

			p += sizeof(struct inotify_event) + event->len;
		}
	}

	return 0;
}

void setupEnvironment(std::string uuid)
{
	std::string uuidPath = OUTPUTROOT + "/" + uuid;
	std::string tmpPath  = uuidPath + "/" + TMPFOLDER;
	std::string dashPath = uuidPath + "/" + DASHFOLDER;
	struct stat st;

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

