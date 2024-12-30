///////////////////////////////////////////////////////////////////////
// File Name : srv.c		                                         //
// Date : 2024/05/22                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #3-1 ( ftp server )          //
// Description : Learn and practice                                  //
//               User authentication/access control	                 //
//				 in side of server									 //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define MAX_BUF 1024

int check_ip(const char *userIP);
int log_auth(int connfd);
int user_match(char *user, char *passwd);

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: 0 -> success	                                             //
// 		  -1 -> fail                                                 //
// 				                                                     //
// Purpose: Open socket												 //
//			Try to connect to server which user wants to enter		 //
// 	     	Call log_in function									 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	int clilen;

	struct passwd *pw;

	char checkIP[MAX_BUF];
	char userIP[MAX_BUF];
	char *ptr;

	if (argc != 2)
	{
		ptr = "enter two arguments!";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return -1;
	}
	// open socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return -1;
	}

	////////// bind socket //////////
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		ptr = "Server: Can't bind\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return -1;
	}

	listen(server_fd, 5);
	while (1)
	{
		clilen = sizeof(client_addr);
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &clilen);

		printf("** Client is trying to connect **\n");
		printf(" - IP:   %s\n", inet_ntoa(client_addr.sin_addr));
		printf(" - Port: %d\n", ntohs(client_addr.sin_port));

		/* Client의 IP가 접근 가능한지 확인 */
		strcpy(userIP, inet_ntoa(client_addr.sin_addr));
		if (check_ip(userIP) == 0)
		{
			printf("** It is NOT authenticated client **\n\n");
			write(client_fd, "REJECTION", MAX_BUF);
			close(client_fd);
			continue;
		}
		write(client_fd, "ACCEPTED", MAX_BUF);
		printf("** Client is connected **\n");

		if (log_auth(client_fd) == 0)
		{
			// if 3 times fail (ok : 1, fail : 0)
			printf("** Fail to log-in **\n\n");
			close(client_fd);
			continue;
		}
		printf("** Success to log-in **\n\n");
		close(client_fd);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// check_ip                                                          //
// ================================================================= //
// Input: char * -> user IP									         //
// 				                                                     //
// Output: 1 -> success	                                             //
// 		   0 -> fail                                                 //
// 				                                                     //
// Purpose: Compare userIP with checkIP in access.txt				 //
///////////////////////////////////////////////////////////////////////
int check_ip(const char *userIP)
{
	FILE *fp_checkIP; // FILE stream to check client’s IP
	char *line;

	char str_uip[MAX_BUF];
	char str_cip[MAX_BUF];
	char uip[4][MAX_BUF];

	char *ptr;
	int i = 0;

	fp_checkIP = fopen("access.txt", "r"); // open access.txt for read only

	strcpy(str_uip, userIP);

	///////////// parsing user ip by "." ////////////////////
	i = 0;
	ptr = strtok(str_uip, ".");
	do
	{
		strcpy(uip[i], ptr);
		i++;
	} while (ptr = strtok(NULL, "."));

	/////////// check if userIP is registered in access.txt ///////////////
	while (line = fgets(str_cip, MAX_BUF, fp_checkIP))
	{
		str_cip[strlen(str_cip) - 1] = '\0';
		i = 0;

		// convert check decimal ip to 32bits integer ////////////////////
		ptr = strtok(str_cip, ".");
		do
		{
			if(!strcmp(ptr, "*") || !strcmp(uip[i], ptr))
				i++;
			else
				break;
		} while (ptr = strtok(NULL, "."));

		if (i == 4)
			break;
	}

	fclose(fp_checkIP);

	if (i == 4)
		return 1;
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////
// log_auth                                                          //
// ================================================================= //
// Input: int -> connection file description				         //
// 				                                                     //
// Output: 1 -> success	                                             //
// 		   0 -> fail                                                 //
// 				                                                     //
// Purpose: Compare user ID and Password with those in passwd		 //
// 			three times                                              //
///////////////////////////////////////////////////////////////////////
int log_auth(int connfd)
{
	char user[MAX_BUF], passwd[MAX_BUF];
	int n, count = 1;
	while (1)
	{
		printf("** User is trying to log-in (%d/3) **\n", count);
		/* username과 password를 client로부터 받는다 */
		read(connfd, user, MAX_BUF);
		read(connfd, passwd, MAX_BUF);

		write(connfd, "OK", MAX_BUF);

		if ((n = user_match(user, passwd)) == 1)
		{
			/* 인증 OK */
			write(connfd, "OK", MAX_BUF);
			break;
		}
		else if (n == 0)
		{
			printf("** Log-in failed **\n");
			/////////// 3 times fail ///////////////
			if (count >= 3)
			{
				write(connfd, "DISCONNECTION", MAX_BUF);
				return 0;
			}
			write(connfd, "FAIL", MAX_BUF);
			count++;
			continue;
		}
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////
// user_match                                                        //
// ================================================================= //
// Input: char * -> user ID									         //
// 		  char * -> user Password                                    //
// 				                                                     //
// Output: 1 -> success	                                             //
// 		   0 -> fail                                                 //
// 				                                                     //
// Purpose: Compare user ID and Password with those in passwd		 //
// 				                                                     //
///////////////////////////////////////////////////////////////////////
int user_match(char *user, char *passwd)
{
	FILE *fp;
	struct passwd *pw;

	fp = fopen("passwd", "r");

	//////////////// get passwd from file "passwd" /////////////////////
	while (pw = fgetpwent(fp))
	{
		//////////////// compare passwd information(user name, password) /////////////////////
		if (!strcmp(pw->pw_name, user) && !strcmp(pw->pw_passwd, passwd))
			break;
	}

	fclose(fp);

	if (pw != NULL)
		return 1;
	else
		return 0;
}