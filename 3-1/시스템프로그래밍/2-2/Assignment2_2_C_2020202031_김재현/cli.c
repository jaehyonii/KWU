///////////////////////////////////////////////////////////////////////
// File Name : cli.c                                       			 //
// Date : 2024/05/08                                                //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #2-2 ( ftp server )          //
// Description : Learn and practice                                  //
//               Socket Programming & process handling               //
//				 in side of client									 //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 256

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: int -> 0		                                             //
// 				                                                     //
// Purpose: recieve string in buff, send it to server				 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	char buff[BUF_SIZE];
	int n;
	int sockfd;
	struct sockaddr_in serv_addr;

	///////////// open socket //////////////
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	///////////// connect to server /////////////
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		n = read(STDIN_FILENO, buff, BUF_SIZE); // read from USER

		buff[n - 1] = '\0';					   // replace new line with null character
		if (write(sockfd, buff, BUF_SIZE) > 0) // write to server
		{
			if (read(sockfd, buff, BUF_SIZE) > 0) // read from server
				printf("from server:%s\n", buff);
			else
				break;
		}
		else
			break;
	}
	close(sockfd); // close socket

	return 0;
}