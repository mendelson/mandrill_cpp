/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "sha1.h"
#include <string>
#include <cmath>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

char *randstring(int length);


int main(int argc, char *argv[])
{
     int g = 4, b = 0, p = 32;
     std::string key = "o monstro tah saindo da jaula";
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);

     // Step 1: Receive authentication request
     // Receive R1 , "Alice"
     n = read(newsockfd,buffer,255);

     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

    std::string r1(buffer);
    r1 = r1.substr(r1.find("|") + 1);

    printf("AAAAAA: %s\n", r1.c_str());
    // Step 2: Send challenge
    // Send R2 + [E("Bob" , R1 , g^b mod p , k)]
    std::string nome = "servidor";
    std::string r2(randstring(16));
    std::string module = std::to_string((int)(pow(g, b)) % p);

    const char *msg = (r2 + "|" + sha1(nome + "|" + r1 + "|" + module + "|" + key)).c_str();

     n = write(newsockfd, msg,  strlen(msg));
    

    // Step 3: Receive challenge response
    // Receive ["Alice" , R2 , g^a mod p , k]
    n = read(newsockfd,buffer,255);
    printf("RECEIVED: %s\n", buffer);



    // if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     close(sockfd);
     return 0; 
}


char *randstring(int length) {    
    static int mySeed = 565454;
    char string[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = strlen(string);        
    char *randomString = NULL;
    int n;

    srand(time(NULL) * length + ++mySeed);

    if (length < 1) {
        length = 1;
    }

    randomString = (char*) malloc(sizeof(char) * (length +1));

    if (randomString) {
        short key = 0;

        for (n = 0;n < length;n++) {            
            key = rand() % stringLen;          
            randomString[n] = string[key];
        }

        randomString[length] = '\0';

        return randomString;        
    }
    else {
        printf("No memory");
        exit(1);
    }
}