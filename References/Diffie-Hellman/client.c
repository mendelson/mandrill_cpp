#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string>
#include <math.h>
#include "sha1.h"


char *randstring(int length);


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int g = 4, a = 0, p = 32;
    char key[] = "o monstro tah saindo da jaula";
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char received[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //printf("Please enter the message: ");

    //bzero(buffer,256);
    //fgets(buffer,255,stdin);

    // Step 1: Request authentication
    //       #Send R1 , "Alice"
    std::string nome = "cliente";
    std::string r1(randstring(16));

    const char *msg = (nome + "|" + r1).c_str();

    n = write(sockfd, msg, strlen(msg));

    // Step 2: Receive the challenge
    // Receive R2 + [E("Bob" , R1 , g^b mod p , k)]
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    std::string r2(buffer);
    r2 = r2.substr(0, r2.find("|"));

    // Step 3: Send challenge response
    // Send ["Alice" , R2 , g^a mod p , k]
    std::string module = std::to_string((int)(pow(g, a)) % p);

    msg = (sha1(nome + "|" + r2 + "|" + module + "|" + key)).c_str();

    n = write(sockfd, msg,  strlen(msg));

    close(sockfd);
    return 0;
}



char *randstring(int length) {    
    static int mySeed = 25011984;
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