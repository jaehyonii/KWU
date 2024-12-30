///////////////////////////////////////////////////////////////////////
// File Name : cli.c                                       			 //
// Date : 2024/05/16                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #2-3 ( ftp server )          //
// Description : Learn and practice                                  //
//               Socket Programming & process handling               //
//				 in side of client									 //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFF 4096

int srv_fd; // server_fd

int conv_cmd(const char *buff, char *cmd_buff);
void sh_int(int);

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: NON			                                             //
// 				                                                     //
// Purpose: recieve USER command in cmd_buff, 		 				 //
// 	     convert it to FTP command, store in ftp_buff				 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	char *ptr; // point string
	struct sockaddr_in srv_addr;

	char cmd_buff[MAX_BUFF], ftp_buff[MAX_BUFF], rcv_buff[MAX_BUFF];
	int len;

	signal(SIGINT, sh_int);

	if (argc != 3)
	{
		ptr = "enter three arguments!";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}
	//////////// memory initialization /////////////////
	memset((char *)&srv_addr, 0, sizeof(srv_addr));
	memset(cmd_buff, 0, MAX_BUFF);
	memset(ftp_buff, 0, MAX_BUFF);
	memset(rcv_buff, 0, MAX_BUFF);

	// open socket
	if ((srv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}

	// connect server
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	srv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(srv_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
	{
		ptr = "can't connect.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}

	write(STDOUT_FILENO, "> ", 2);
	while ((len = read(STDIN_FILENO, cmd_buff, MAX_BUFF)) > 0)
	{
		cmd_buff[--len] = '\0';

		/* convert ls (including options) to NLST (including options) */
		if (conv_cmd(cmd_buff, ftp_buff) < 0)
		{
			ptr = "conv_cmd() error!!\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
			exit(1);
		}

		len = strlen(ftp_buff);
		if (write(srv_fd, ftp_buff, len) != len)
		{
			ptr = "write() error!!\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
			exit(1);
		}

		/* quit program */
		if (read(srv_fd, rcv_buff, MAX_BUFF) <= 0)
			break;

		/* display ls(including options) command result */
		write(STDOUT_FILENO, rcv_buff, strlen(rcv_buff));

		write(STDOUT_FILENO, "\n> ", 3);

		memset(cmd_buff, 0, MAX_BUFF);
		memset(ftp_buff, 0, MAX_BUFF);
		memset(rcv_buff, 0, MAX_BUFF);
	}

	close(srv_fd);

	return 0;
}

///////////////////////////////////////////////////////////////////////
// conv_cmd                                                          //
// ================================================================= //
// Input: const char * -> cmd 						                 //
// 		  char * -> ftp			                                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: recieve USER command in cmd_buff, 		 				 //
// 	     convert it to FTP command, store in ftp_buff				 //
///////////////////////////////////////////////////////////////////////
int conv_cmd(const char *cmd_buff, char *ftp_buff)
{
	int argc = 0;
	char *argv[100];
	char cmd[MAX_BUFF];
	char *ptr;

	memcpy(cmd, cmd_buff, MAX_BUFF);
	if (ptr = strtok(cmd, " "))
	{
		argv[0] = ptr;
		argc++;
	}
	while (ptr = strtok(NULL, " "))
	{
		argv[argc] = ptr;
		argc++;
	}

	// no USER command
	if (argc == 0)
		strncpy(ftp_buff, "NON", 4);
	// ls -> NLST
	else if (!strcmp((argv[0]), "ls"))
		strncpy(ftp_buff, "NLST", 5);
	// dir -> LIST
	else if (!strcmp((argv[0]), "dir"))
		strncpy(ftp_buff, "LIST", 5);
	// pwd -> PWD
	else if (!strcmp((argv[0]), "pwd"))
		strncpy(ftp_buff, "PWD", 4);
	// cd -> CWD or CDUP
	else if (!strcmp((argv[0]), "cd"))
	{
		// cd -> CWD
		strncpy(ftp_buff, "CWD", 3);

		for (int i = 1; i < argc; i++)
		{
			// cd -> CDUP
			if (!strcmp(argv[i], ".."))
			{
				strncpy(ftp_buff, "CDUP", 5);
				// remove argument ".."
				for (int j = i; j < argc; j++)
					argv[j] = argv[j + 1];
				argc--;
				break;
			}
		}
	}
	// mkdir -> MKD
	else if (!strcmp((argv[0]), "mkdir"))
		strncpy(ftp_buff, "MKD", 4);
	// delete -> DELE
	else if (!strcmp((argv[0]), "delete"))
		strncpy(ftp_buff, "DELE", 5);
	// rmdir -> RMD
	else if (!strcmp((argv[0]), "rmdir"))
		strncpy(ftp_buff, "RMD", 4);
	// rename -> RNFR & RNTO
	else if (!strcmp((argv[0]), "rename"))
		sprintf(ftp_buff, "RNFR %s RNTO", argv[1]);
	// quit -> QUIT
	else if (!strcmp((argv[0]), "quit"))
		strncpy(ftp_buff, "QUIT", 5);
	// USER command is incorrect
	else
		strncpy(ftp_buff, "WRONG", 6);

	// write arguments to the buffer
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[0], "rename") && i < 2)
			continue;
		strcat(ftp_buff, " ");
		strcat(ftp_buff, argv[i]);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// sh_int                                   				         //
// ================================================================= //
// Input: int -> signum		 						                 //
// 				                                                     //
// Output: NON				                                         //
// 				                                                     //
// Purpose: when interrupt occurs,									 //
// 	        send "QUIT" to server							         //
//			close socket											 //
//			terminate process										 //
///////////////////////////////////////////////////////////////////////
void sh_int(int signum)
{
	char *ptr;

	if (write(srv_fd, "QUIT", 4) != 4)
	{
		ptr = "interrupt handling error!!\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
	}
	close(srv_fd);
	exit(1);
}