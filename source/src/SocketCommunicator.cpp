#include "SocketCommunicator.hpp"
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

SocketCommunicator *SocketCommunicator::_instance = 0;
std::mutex SocketCommunicator::_instanceMutex;

SocketCommunicator *SocketCommunicator::getSocket()
{
	std::unique_lock<std::mutex> _lock(_instanceMutex);

	if(_instance == 0)
	{
		_instance = new SocketCommunicator();
	}

	return _instance;
}

void SocketCommunicator::notifyNewFootage()
{
	// int n;

	//    _linkToStreaming = "link_louco_aqui";

	//    printf("Please enter the message: ");
	//    bzero(_buffer, BUFFERSIZE);
	//    fgets(_buffer, BUFFERSIZE - 1, stdin);
	//    n = write(_socketFileDescriptor, _linkToStreaming.c_str(),
	//    _linkToStreaming.size());

	int n;

	_notification = "Chegou nova filmagem!";

	n = write(_socketFileDescriptor, _notification.c_str(),
			  _notification.size());

	if(n < 0)
	{
		std::cout << "ERROR writing to socket" << std::endl;
		exit(-7);
	}

	// bzero(_buffer, 256);
	// n = read(_socketFileDescriptor, _buffer, 255);

	// if(n < 0)
	// 	std::cout << "ERROR reading from socket" << std::endl;

	// printf("%s\n", _buffer);
	// close(_socketFileDescriptor);
}

void SocketCommunicator::setupSocket(int portNumber)
{
	struct hostent *server;
	struct sockaddr_in serv_addr;

	_socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(_socketFileDescriptor < 0)
	{
		std::cout << "ERROR opening socket." << std::endl;
		exit(-8);
	}

	server = gethostbyname(HOSTNAME);

	if(server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(-9);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
		  server->h_length);

	serv_addr.sin_port = htons(portNumber);

	if(connect(_socketFileDescriptor, (struct sockaddr *)&serv_addr,
			   sizeof(serv_addr)) < 0)
	{
		std::cout << "ERROR connecting" << std::endl;
		exit(-10);
	}
}

SocketCommunicator::~SocketCommunicator()
{
	close(_socketFileDescriptor);
}

// void error(const char *msg)
// {
//  perror(msg);
//  exit(0);
// }
// char *randstring(int length);


// int main(int argc, char *argv[])
// {

// }


// char *randstring(int length)
// {
// 	static int mySeed = 25011984;
// 	char string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ012345"
// 					"6789";
// 	size_t stringLen   = strlen(string);
// 	char *randomString = NULL;
// 	int n;

// 	srand(time(NULL) * length + ++mySeed);

// 	if(length < 1)
// 	{
// 		length = 1;
// 	}

// 	randomString = (char *)malloc(sizeof(char) * (length + 1));

// 	if(randomString)
// 	{
// 		short key = 0;

// 		for(n = 0; n < length; n++)
// 		{
// 			key				= rand() % stringLen;
// 			randomString[n] = string[key];
// 		}

// 		randomString[length] = '\0';

// 		return randomString;
// 	}
// 	else
// 	{
// 		printf("No memory");
// 		exit(1);
// 	}
// }

