///////////////////////////////////////////////////////////////////////
// File Name : srv.c		                                         //
// Date : 2024/05/08                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #2-2 ( ftp server )          //
// Description : Learn and practice                                  //
//               Socket Programming & process handling               //
//				 in side of server									 //
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

void sh_chld(int);								 // signal handler for SIGCHLD
void sh_alrm(int);								 // signal handler for SIGALRM
int print_cli_info(struct sockaddr_in cli_addr); // print client info
void print_chd_pid(int pid);					 // print child's pid

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: int -> 0 		                                         //
// 				                                                     //
// Purpose: recieve string from user, send it as is to Client		 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	char buff[BUF_SIZE];
	int n;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	int len;
	int port;

	signal(SIGCHLD, sh_chld);
	signal(SIGALRM, sh_alrm);

	////////// open socket //////////
	server_fd = socket(PF_INET, SOCK_STREAM, 0);

	////////// bind socket //////////
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	////////// ready to accept clients /////////////
	listen(server_fd, 5);

	while (1)
	{
		pid_t pid;
		len = sizeof(client_addr);
		////////// accept client //////////
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);

		pid = fork();
		if (pid > 0) // parent
		{
			print_cli_info(client_addr); // print client information(ip, port)
			print_chd_pid(pid);			 // print child's process id
		}
		else if (pid == 0) // child
		{
			while (1)
			{
				read(client_fd, buff, BUF_SIZE); // read from Client
				if (!strcmp(buff, "QUIT"))		 // if string is "QUIT"
					alarm(1);					 // set alarm 1sec
				else
					write(client_fd, buff, BUF_SIZE); // send string as is to Client
			}
		}

		close(client_fd); // close client socket
	}

	close(server_fd); // close server socket

	return 0;
}

// when child's status changed(terminated), get child's terminate statuseex
void sh_chld(int signum)
{
	printf("Status of Child process was changed.\n");
	wait(NULL);
}

// exit process when alarm signal occurs
void sh_alrm(int signum)
{
	printf("Child Process(PID : %d) will be terminated.\n", getpid());
	exit(1);
}

// print clients' information
int print_cli_info(struct sockaddr_in cli_addr)
{
	char buf[BUF_SIZE];
	int j = 0;

	j = sprintf(buf, "==========Client info==========\n");
	j += sprintf(buf + j, "client IP: %s\n\n", inet_ntoa(cli_addr.sin_addr));
	j += sprintf(buf + j, "client port: %d\n", ntohs((int)cli_addr.sin_port));
	j += sprintf(buf + j, "===============================\n");

	return write(STDOUT_FILENO, buf, strlen(buf));
}

// print child's process id
void print_chd_pid(int pid)
{
	char buf[BUF_SIZE];

	sprintf(buf, "Child Process ID : %d\n", pid);
	write(STDOUT_FILENO, buf, strlen(buf));
}