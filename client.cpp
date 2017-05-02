#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<iostream>
#include<fstream>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char str[1024];
	printf("Enter a server host name: ");
	cin>>str;
	argv[1] = str;
	printf("Enter server port number: ");
	cin >> portno;
    while (true){
    char buffer[256];
//    if (argc < 3) {
//       fprintf(stderr,"usage %s hostname port\n", argv[0]);
//       exit(0);
//    }

    //portno = atoi(argv[2]);
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

    printf("Enter your username: ");
    bzero(buffer,256);
    cin>>buffer;


    //if (strcmp(buffer,"\n")==0)
    if (strcmp(buffer,"Terminate")==0)
    {
    	n = write(sockfd, buffer, strlen(buffer));
    	printf("Exit.\n");
    	break;
    }
    else
    {
//    	cout<<buffer<<endl;
//		buffer[strlen(buffer) - 1] = '\0';
//		cout<<buffer<<endl;

		}
		n = write(sockfd, buffer, strlen(buffer));
		char username[256];
		strncpy(username, buffer, 256);
		if (n < 0)
			error("ERROR writing to socket");
		bzero(buffer, 256);

		n = read(sockfd, buffer, 255);

		if (n < 0)
			error("ERROR reading from socket");
		printf("--------------------------------------------\n");
		printf("%s, this user's public key is: %s\n", username, buffer);
		printf("--------------------------------------------\n");
		close(sockfd);
	}
	return 0;
}
