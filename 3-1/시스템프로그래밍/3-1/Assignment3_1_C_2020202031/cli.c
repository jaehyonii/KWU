///////////////////////////////////////////////////////////////////////
// File Name : cli.c                                       			 //
// Date : 2024/05/22                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #3-1 ( ftp server )          //
// Description : Learn and practice                                  //
//               User authentication/access control	                 //
//				 in side of client									 //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUF 1024
#define CONT_PORT 20001

void log_in(int server_fd);

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: 0			                                             //
// 				                                                     //
// Purpose: Open socket												 //
//			Try to connect to server which user wants to enter		 //
// 	     	Call log_in function									 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	int server_fd, n, p_pid;
	struct sockaddr_in server_addr;
	char *ptr;

	if (argc != 3)
	{
		ptr = "enter three arguments!";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}

	// open socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}

	// connect server
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	log_in(server_fd);
	close(server_fd);
	return 0;
}

///////////////////////////////////////////////////////////////////////
// log_in                                                            //
// ================================================================= //
// Input: int -> server file descripter				                 //
// 				                                                     //
// Output: NON			                                             //
// 				                                                     //
// Purpose: Communicate with server									 //
//			Whether IP rejected or accepted							 //
// 	     	Whether ID, Password correct or	wrong					 //
///////////////////////////////////////////////////////////////////////
void log_in(int server_fd)
{
	int n;
	char user[MAX_BUF], buf[MAX_BUF];
	char *passwd;

	/* Check if the ip is acceptable */
	read(server_fd, buf, MAX_BUF);
	if (!strcmp(buf, "REJECTION"))
		printf("** Connection refused **\n");
	else if (!strcmp(buf, "ACCEPTED"))
	{
		printf("** It is connected to Server **\n");
		while (1)
		{
			/* get username from user */
			write(STDOUT_FILENO, "Input ID : ", 11);
			n = read(STDIN_FILENO, user, MAX_BUF);
			user[n - 1] = '\0';
			/* get password from user */
			passwd = getpass("Input Password : ");

			/* pass username to server */
			write(server_fd, user, MAX_BUF);
			/* pass password to server */
			write(server_fd, passwd, MAX_BUF);

			read(server_fd, buf, MAX_BUF);

			if (!strcmp(buf, "OK"))
			{
				read(server_fd, buf, MAX_BUF);

				if (!strcmp(buf, "OK"))
				{
					/* login success */
					printf("** User '%s' logged in **\n", user);
					break;
				}
				else if (!strcmp(buf, "FAIL"))
				{
					/* login fail */
					printf("** Log-in failed **\n");
					continue;
				}
				else
				{ // buf is “DISCONNECTION”
					/* three times fail */
					printf("** Connection closed **\n");
					break;
				}
			}
		}
	}
}