#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFF 4096

int conv_cmd(const char *buff, char *cmd_buff);

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
void main(int argc, char **argv)
{
	char *ptr; // point string
	struct sockaddr_in srv_addr;
	int srv_fd;
	char cmd_buff[MAX_BUFF], ftp_buff[MAX_BUFF], rcv_buff[MAX_BUFF];
	int len;

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
	while ((len = read(STDIN_FILENO, cmd_buff, sizeof(cmd_buff))) > 0)
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

		if ((len = read(srv_fd, rcv_buff, MAX_BUFF - 1)) < 0)
		{
			ptr = "read() error!!\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
			exit(1);
		}

		/* quit program */
		if (!strcmp(rcv_buff, "QUIT"))
		{
			ptr = "Program quit!!\n";
			write(STDOUT_FILENO, ptr, strlen(ptr));
			exit(1);
		}
		/* display ls(including options) command result */
		write(STDOUT_FILENO, rcv_buff, strlen(rcv_buff));

		write(STDOUT_FILENO, "\n> ", 3);

		memset(cmd_buff, 0, MAX_BUFF);
		memset(ftp_buff, 0, MAX_BUFF);
		memset(rcv_buff, 0, MAX_BUFF);
	}
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
	{
		strncpy(ftp_buff, "NON", 4);
	}
	// ls -> NLST
	else if (!strcmp((argv[0]), "ls"))
	{
		strncpy(ftp_buff, "NLST", 5);
	}
	// quit -> QUIT
	else if (!strcmp((argv[0]), "quit"))
	{
		strncpy(ftp_buff, "QUIT", 5);
	}
	// USER command is incorrect
	else
	{
		strncpy(ftp_buff, "WRONG", 6);
	}

	// write arguments to the buffer
	for (int i = 1; i < argc; i++)
	{
		strcat(ftp_buff, " ");
		strcat(ftp_buff, argv[i]);
	}

	return 0;
}