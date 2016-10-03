#include <iostream>
#include <limits.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef __linux
	#include <sys/inotify.h>
	#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#else
	#define BUF_LEN (10 * (NAME_MAX + 1))
#endif



const std::string OUTPUTROOT = "files";
const std::string TMPFOLDER  = "tmp";
const std::string DASHFOLDER = "dash";
const std::string SPLITTIME  = "2";  // Seconds

void setupEnvironment(std::string uuid);
int watcher_init (std::string path);
void watch_path(int fd, std::string path);


int main(int argc, char *argv[])
{
	if(argc != 7)
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
	std::string IP(argv[5]);
	std::string password(argv[6]);
	int watcherFd;

	std::cout << "Socket port: " << socket << std::endl;
	/////////

	int counter = 0;
	char buf[BUF_LEN];
	std::string str;
	std::string videoString;
	//char MP4BoxPath[]	= "/cygdrive/e/Program Files/GPAC/mp4box";
	char MP4BoxPath[]	= "/usr/local/bin/MP4Box";
	//char gstLaunchPath[] = "/cygdrive/e/gstreamer/1.0/x86_64/bin/gst-launch-1.0";
	char gstLaunchPath[] = "/usr/bin/gst-launch-1.0";
	char smbLinuxPath[] = "/usr/bin/scripts/s";

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

#ifdef __linux
	watcherFd = inotify_init();
	if(watcherFd == -1)
	{
		std::cout << "inotify_init failed" << std::endl;
		exit(1);
	}

	if(inotify_add_watch(watcherFd, tmpPath.c_str(), IN_CLOSE_WRITE) == -1)
	{
		std::cout << "inotify_add_watch failed" << std::endl;
		exit(2);
	}
#else
	watcherFd = watcher_init(tmpPath);
	if(watcherFd == -1)
	{
		std::cout << "watcher_init failed" << std::endl;
		exit(3);
	}
#endif

	int status;
	struct pollfd ufds[2];

	ufds[0].fd	 = watcherFd;
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

		int numRead = read(watcherFd, buf, BUF_LEN);

		if (numRead <= 0)
		{
			std::cout << "read failed" << std::endl;
			exit(numRead);
		}

		for (char *p = buf; p < buf + numRead; )
		{
#ifdef __linux
			struct inotify_event *event = (struct inotify_event *)p;
			std::string filename = std::string(event->name);
			p += sizeof(struct inotify_event) + event->len;
#else
			int size = strlen(p)+1;
			std::string filename = std::string(p, size);
			p += size;
#endif

			std::cout << "dash: " << filename << std::endl;
			videoString = tmpPath + "/" + filename;
			std::cout << "String: " << videoString << "\n";
			/* TODO: filtrar entrada, checar padrao: "video%d.mp4" */
			//callMP4Box(event->name);
			pid_t pid = fork(); /* Create a child process */

			switch(pid)
			{
				case -1: /* Error */
					std::cout << "Uh-Oh! fork() failed.\n";
					break;

				case 0: /* Child process */

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



		}


/*			// sends back up
			#ifdef __linux
			pid_t pid_back_up = fork(); 

			switch(pid_back_up)
			{
				case -1: 
					std::cout << "Uh-Oh! fork() failed.\n";
					break;

				case 0:
					std::cout << "Backup called.\n";

					execl(smbLinuxPath, smbLinuxPath, IP.c_str(),  videoString.c_str(), 
							uuid.c_str(), password.c_str(), NULL);
					
                                        std::cout << "Backup called DEBUG.\n";

					break;

				default: 
					std::cout << "Process created with pid " << pid_back_up << "\n";
					int status;

					while(!WIFEXITED(status))
					{
						
						waitpid(pid_back_up, &status, 0);
					}

					std::cout << "Process exited with " << WEXITSTATUS(status)
						<< "\n";

					break;
			}

			#endif
*/
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

int watcher_init (std::string path)
{
	int pfds[2];

	if(pipe(pfds))
	{
		perror("pipe");
		exit(-5);
	}

	pid_t pid = fork();

	switch (pid)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! watcher fork() failed.\n";
			break;

		case 0: /* Child process */

			close(pfds[0]);
			watch_path(pfds[1], path);

			break;

		default: /* Parent process */
			std::cout << "Watcher process created with pid " << pid << "\n";

			close(pfds[1]);

			return pfds[0];

	}

	exit(-3);
}

void watch_path(int fd, std::string path)
{
	int counter = 1;
	struct stat st;
	std::string oldFile, newFile, fullPath;

	while(1)
	{

		newFile = "video" + std::to_string(counter) + ".mp4";
		fullPath = path + "/" + newFile;

		std::cout << "Looking for file: " << fullPath << std::endl;

		if (stat(fullPath.c_str(), &st) == 0 &&
				abs(st.st_mtime - time(0)) < 22 )
		{
			if (!oldFile.empty())
				write(fd, oldFile.c_str(), oldFile.size() + 1);

			counter++;
			oldFile = newFile;
		}

		sleep(1);
	}

}
