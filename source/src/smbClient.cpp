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
#include <cstring>
#include <sys/stat.h>
#include <string>
#include <algorithm>


void sendFileLinux(std::string nasPath, std::string filePath, std::string newName, std::string uuid, std::string authentication);
void removeFileLinux(std::string nasPath, std::string filePath, std::string authentication);
int checkFileLinux(std::string nasPath, std::string filePath, std::string authentication);
void renameFileLinux(std::string nasPath, std::string filePath, std::string newName, std::string authentication);
void trim(std::string& s);
void createUuidFolder(std::string nasPath, std::string folderName, std::string authentication);
void sendFileWindows(std::string nasPath, std::string filePath, std::string newName, std::string uuid, std::string authentication);
void createUuidFolderWindows(std::string nasPath, std::string folderName, std::string authentication);


int main(int argc, char* argv[]){
	if(argc != 5){
		return -1;
	}

	// nas parameters
	std::string IP(argv[1]); //"10.190.60.69";
	std::string folder = "Teste"; //mudar para mandrill
	std::string uuid(argv[3]);//"birl";
	std::string nasPath = "//" + IP + "/" + folder;
	std::string filePath(argv[2]);//"ace.mp3";
	std::string newName = "";
	std::string pass(argv[4]);
	struct stat st;

	#ifdef __linux
		// check if files is on NAS
		if(checkFileLinux(nasPath, filePath, pass)){
			// tries to open file for renaming
			if(stat(filePath.c_str(), &st) == -1)
		    {
		      exit(-1);
		    }

		    if(checkFileLinux(nasPath, uuid, pass)){
		    	createUuidFolder(nasPath, uuid, pass);
		    	sleep(1);
		    }

			// rename file
			std::ostringstream ss;
			ss << st.st_mtime;
			std::string s(ss.str());
			newName =  s + ".mp4";

			// send file to nas via smb
			sendFileLinux(nasPath, filePath, newName ,uuid, pass);
		}else{
			std::cout << "File already on NAS" << std::endl;
		}

		// remove file on nas
		// removeFileLinux(nasPath, filePath, pass);
		// renameFileLinux(nasPath, filePath, newName, pass);
	#else
		createUuidFolderWindows(nasPath, uuid, pass);
		sleep(1);
		sendFileWindows(nasPath, filePath, newName ,uuid, pass);
	#endif

	return 0;
}




void sendFileLinux(std::string nasPath, std::string filePath, std::string newName, std::string uuid, std::string authentication){
	char smbclientPath[]	= "/usr/bin/smbclient";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// sets up authentication info
	sendCommand = "cd " + uuid + "; put " + filePath + " " + newName;

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(smbclientPath, smbclientPath, nasPath.c_str(), "-U",
				  authentication.c_str(), "-c", sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}


}

void removeFileLinux(std::string nasPath, std::string filePath, std::string authentication){
	char smbclientPath[]	= "/usr/bin/smbclient";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// sets up authentication info
	sendCommand = "rm " + filePath + "";

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(smbclientPath, smbclientPath, nasPath.c_str(), "-U",
				  authentication.c_str(), "-c", sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}

}


int checkFileLinux(std::string nasPath, std::string filePath, std::string authentication){
	std::string smbclientPath = "/usr/bin/smbclient";
	std::string sendCommand;
	FILE *in;
	char buff[512];

	// command to list files in folder
	sendCommand = smbclientPath + " " + nasPath + " -U " + authentication + " -c ls";

	// calls NAS listing
	if(!(in = popen(sendCommand.c_str(), "r"))){
        exit(-1); // fail to list dir
    }

    // parses ls fules names
    while(fgets(buff, sizeof(buff), in)!=NULL){
        // parses name from ls response
        std::string stringAux(buff);
        trim(stringAux);
        stringAux = stringAux.substr(0, stringAux.find_last_of('.'));

        // compares received filename with files of NAS folder
        if(strcmp(stringAux.c_str(), filePath.substr(0, filePath.find_last_of('.')).c_str()) == 0){
        	return 0; // file already o NAS
        }
    }

    pclose(in);


    return 1;
}


void trim(std::string& s){
      size_t p = s.find_first_not_of(" \t");
      s.erase(0, p);
  
      p = s.find_last_not_of(" \t");
      if (std::string::npos != p)
         s.erase(p+1);
}


void renameFileLinux(std::string nasPath, std::string filePath, std::string newName, std::string authentication){
	char smbclientPath[]	= "/usr/bin/smbclient";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// sets up authentication info
	sendCommand = "rename " + filePath + " " + newName;

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(smbclientPath, smbclientPath, nasPath.c_str(), "-U",
				  authentication.c_str(), "-c", sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}

}




void createUuidFolder(std::string nasPath, std::string folderName, std::string authentication){
	char smbclientPath[]	= "/usr/bin/smbclient";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// sets up authentication info
	sendCommand = "mkdir " + folderName;

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(smbclientPath, smbclientPath, nasPath.c_str(), "-U",
				  authentication.c_str(), "-c", sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}

}


void sendFileWindows(std::string nasPath, std::string filePath, std::string newName, std::string uuid, std::string authentication){
	char command[]	= "copy";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// replaces slashes
	std::replace( nasPath.begin(), nasPath.end(), '/', '\\');

	// sets up authentication info
	sendCommand = filePath + " " + nasPath + "\\" + newName;

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(command, command, sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}


}

void createUuidFolderWindows(std::string nasPath, std::string folderName, std::string authentication){
	char command[]	= "mkdir";
	std::string sendCommand;
	pid_t pid_smb = fork(); /* Create a child process */

	// replaces slashes
	std::replace( nasPath.begin(), nasPath.end(), '/', '\\');

	// sets up authentication info
	sendCommand = nasPath + "\\" + folderName;

	std::cout << sendCommand << std::endl;
	// calls smbclient send file process
	switch(pid_smb)
	{
		case -1: /* Error */
			std::cout << "Uh-Oh! fork() failed.\n";
			break;

		case 0: /* Child process */

			execl(command, command, sendCommand.c_str(), NULL);
			break;

		default: /* Parent process */
			std::cout << "Process created with pid_smb " << pid_smb
					  << "\n";
			break;
	}

}

