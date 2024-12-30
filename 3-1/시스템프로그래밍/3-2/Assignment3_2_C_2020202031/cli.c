///////////////////////////////////////////////////////////////////////
// File Name : cli.c                                       			 //
// Date : 2024/05/30                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #3-2 ( ftp server )          //
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
#include <time.h>

#define MAX_BUFF 4096

char *convert_addr_to_str(unsigned long ip_addr, unsigned int port);
void conv_cmd(const char *cmd_buff, char *ftp_buff);

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: NON			                                             //
// 				                                                     //
// Purpose: Open socket												 //
//			Try to connect to server which user wants to enter		 //
// 	     	send ftp command by control connect stream				 //
// 			receive the result of ftp command by data connect stream //
///////////////////////////////////////////////////////////////////////
void main(int argc, char **argv)
{
	char *ptr;		   // string pointer for STDOUT write
	char *hostport;	   // port command
	unsigned int port; // port for data connect

	int ctrl_server_fd; // control connect
	struct sockaddr_in ctrl_server_addr;

	int data_server_fd, data_client_fd; // data connect
	struct sockaddr_in data_server_addr, data_client_addr;
	int clilen;

	char buff[MAX_BUFF];	  // buff for STDIN
	char ftp_buff[MAX_BUFF];  // buff for result of converting user command to ftp command
	char ctrl_buff[MAX_BUFF]; // buff for sending to control connect server
	char data_buff[MAX_BUFF]; // buff for receiving from data connect client

	/* your own codes */
	if (argc != 3)
	{
		ptr = "enter three arguments!";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}
	// open socket
	if ((ctrl_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		exit(1);
	}

	/////////////// control connection to server /////////////////
	memset((char *)&ctrl_server_addr, 0, sizeof(ctrl_server_addr));
	ctrl_server_addr.sin_family = AF_INET;
	ctrl_server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ctrl_server_addr.sin_port = htons(atoi(argv[2]));
	if (connect(ctrl_server_fd, (struct sockaddr *)&ctrl_server_addr, sizeof(ctrl_server_addr)) < 0)
	{ // error
		ptr = "control connection fails\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return;
	}

	srand(time(NULL));
	while (1)
	{
		// memory initiating
		memset(buff, 0, sizeof(buff));
		memset(ftp_buff, 0, sizeof(ftp_buff));
		memset(ctrl_buff, 0, sizeof(ctrl_buff));
		memset(data_buff, 0, sizeof(data_buff));

		write(STDOUT_FILENO, "> ", 2);
		// read USER command input
		if (read(STDIN_FILENO, buff, MAX_BUFF) < 0)
		{
			ptr = "read error!";
			write(STDERR_FILENO, ptr, strlen(ptr));
			exit(1);
		}
		buff[strlen(buff) - 1] = '\0';

		// convert USER command to FTP command
		conv_cmd(buff, ftp_buff);

		if (!strcmp(ftp_buff, "NON"))
		{ // USER don't input anyting (only '\n')
			ptr = "Non Command!\n\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
			continue;
		}
		else if (!strcmp(ftp_buff, "WRONG"))
		{ // USER input invalid command
			ptr = "Invalid Command!\n\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
			continue;
		}

		if (!strcmp(ftp_buff, "QUIT"))
		{ // USER input quit command
			write(ctrl_server_fd, "QUIT", 5);
			read(ctrl_server_fd, ctrl_buff, MAX_BUFF);
			printf("%s\n", ctrl_buff);
			break;
		}
		else
		{
			////////////////// make data connection //////////////////////
			port = rand() % 20000 + 10001; // 10001 <= port <= 30000
			if ((data_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				ptr = "Server: Can't open stream socket.\n";
				write(STDERR_FILENO, ptr, strlen(ptr));
				break;
			}
			memset(&data_server_addr, 0, sizeof(data_server_addr));
			data_server_addr.sin_family = AF_INET;
			data_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			data_server_addr.sin_port = htons(port);
			if (bind(data_server_fd, (struct sockaddr *)&data_server_addr, sizeof(data_server_addr)) < 0)
			{
				ptr = "Server: Can't bind\n";
				write(STDERR_FILENO, ptr, strlen(ptr));
				break;
			}
			listen(data_server_fd, 5);
			/////////////////////////////////////////////////////////////////
			////////////////// accept data connection ///////////////////////
			hostport = convert_addr_to_str(data_server_addr.sin_addr.s_addr, data_server_addr.sin_port);
			printf("converting to %s\n", hostport);
			write(ctrl_server_fd, hostport, MAX_BUFF); // send PORT command
			free(hostport);
			clilen = sizeof(data_client_addr);
			data_client_fd = accept(data_server_fd, (struct sockaddr *)&data_client_addr, &clilen);
			////////////////////////////////////////////////////////////////

			read(ctrl_server_fd, ctrl_buff, MAX_BUFF); // receive code frome server
			if (!strcmp(ctrl_buff, "200 Port command performed successfully."))
				printf("%s\n", ctrl_buff);
			else // receive "550 Failed to access."
			{
				printf("%s\n", ctrl_buff);
				close(data_server_fd);
				break;
			}

			write(ctrl_server_fd, ftp_buff, MAX_BUFF); // send ftp command to server

			read(ctrl_server_fd, ctrl_buff, MAX_BUFF); // receive code frome server
			printf("%s\n", ctrl_buff);

			read(data_client_fd, data_buff, MAX_BUFF); // receive the result of ftp command
			printf("%s", data_buff);
			close(data_server_fd); // close data stream
			close(data_client_fd); // close data stream

			read(ctrl_server_fd, ctrl_buff, MAX_BUFF); // receive code frome server
			if (!strcmp(ctrl_buff, "226 Complete transmission."))
				printf("%s\n", ctrl_buff);
			else // receive "550 Failed transmission."
			{
				printf("%s\n", ctrl_buff);
				break;
			}

			printf("OK. %ld bytes is received.\n\n", strlen(data_buff));
		}
	}
	close(ctrl_server_fd); // close control stream

	return;
}

///////////////////////////////////////////////////////////////////////
// convert_addr_to_str                                               //
// ================================================================= //
// Input: unsigned long -> ip_addr 					                 //
// 		  unsigned int -> port	                                     //
// 				                                                     //
// Output: char * -> PORT command									 //
// 				                                                     //
// Purpose: recieve 4 byte Integer ip, 2 byte Integer port			 //
// 	     	convert it to PORT command								 //
///////////////////////////////////////////////////////////////////////
char *convert_addr_to_str(unsigned long ip_addr, unsigned int port)
{ // 자신의 IP주소와 임의의 포트번호를 PORT명령어에 붙는 형태로 변경
	char *cmd_port;
	int j = 0;

	/* your converting algorithm */
	ip_addr = ntohl(ip_addr); // conver ip to host byte order
	port = ntohs(port);		  // conver port to host byte order
	cmd_port = (char *)malloc(sizeof(char) * 30);
	j += sprintf(cmd_port + j, "PORT ");
	for (int i = 3; i >= 0; i--)
		j += sprintf(cmd_port + j, "%lu,", (ip_addr & (0xFF << (8 * i))) >> 8 * i);
	j += sprintf(cmd_port + j, "%u,%u", (port & 0xFF00) >> 8, port & 0x00FF);

	return cmd_port;
}

///////////////////////////////////////////////////////////////////////
// conv_cmd                                                          //
// ================================================================= //
// Input: const char * -> cmd 						                 //
// 		  char * -> ftp			                                     //
// 				                                                     //
// Output: NON														 //
// 				                                                     //
// Purpose: recieve USER command in cmd_buff, 		 				 //
// 	     convert it to FTP command, store in ftp_buff				 //
///////////////////////////////////////////////////////////////////////
void conv_cmd(const char *cmd_buff, char *ftp_buff)
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
}