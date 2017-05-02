/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<fstream>
#include<string>
#include<iostream>

using namespace std;

struct publicKeyList{
	
	char username[256];
	char publicKey[256];
};
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     char bufferReturn[256];
     //string bufferReturn;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     printf("Enter server port number: ");
     cin>>portno;

     //if (argc < 2) {}
     if (portno == 0){
         fprintf(stderr,"ERROR, port is wrong!\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //portno = atoi(argv[1]);

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     ifstream file;
	while (true) {
		cout << "Please enter file name of the user list: ";
		string filename;
		cin >> filename;
		printf("---------------------------------------------\n");
		file.open(filename.c_str());
		if (file.is_open())
			break;
	}

	char username[256];
	char publicKey[256];
	publicKeyList publicKeyList[1024];
	int i=0;

	while (!file.eof())
	{
		file>> username;
		strncpy(publicKeyList[i].username, username,256);

		file>> publicKey;

		strncpy(publicKeyList[i].publicKey, publicKey,256);
		i++;
	}

	int visited[1024];
	int visitedCount=0;
	 while (true) {
		listen(sockfd, 5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
			error("ERROR reading from socket");

		string line;
		string msg=buffer;

		//msg.erase(remove(msg.begin(), msg.end(), '\n'), msg.end());
		int j=0;
		bool found=false;

		while (j<i)
		{
			if (publicKeyList[j].username ==msg)
			{
				strncpy(bufferReturn,publicKeyList[j].publicKey,256);
				found=true;
				visited[visitedCount]=j;
				visitedCount++;
				break;
			}
			j++;
		}
		if (found)
		{
			printf("Here is the publicj key for %s: %s\n", buffer, bufferReturn);
			printf("---------------------------------------------\n");
		}
		else if (msg=="Terminate"){
			for (int i=0;i<visitedCount;i++)
			{
				printf("%s %s\n",publicKeyList[visited[i]].username,publicKeyList[visited[i]].publicKey);
			}
			printf("Done!\n");
			printf("--------------------------------------------\n");
			for (int i=0;i<1024;i++)
					{
						visited[i]=-1;
					}
					visitedCount=0;
		}
		else {
			printf("Invalid username.\n");
			strncpy(bufferReturn, "not available.", 256);
			printf("--------------------------------------------\n");
		}
		n = write(newsockfd, bufferReturn, 100);
		if (n < 0)
			error("ERROR writing to socket");
		close(newsockfd);

	}
     return 0;
}
