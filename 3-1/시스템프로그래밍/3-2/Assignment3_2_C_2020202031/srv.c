///////////////////////////////////////////////////////////////////////
// File Name : srv.c		                                         //
// Date : 2024/05/30                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #3-2 ( ftp server )          //
// Description : Learn and practice                                  //
//               User authentication/access control	                 //
//				 in side of server									 //
///////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#define MAX_BUFF 4096

char *convert_str_to_addr(const char *str, unsigned int *port);
////////////////// cmd_process ////////////////////////////
int cmd_process(const char *buff, char *result_buff);
int NLST(char *result_buff, const char *pathname, int opflag);
int LIST(char *result_buff, const char *pathname);
int PWD(char *result_buff);
int CWD(char *result_buff, const char *pathname);
int CDUP(char *result_buff);
int MKD(char *result_buff, const char *pathname);
int DELE(char *result_buff, const char *pathname);
int RMD(char *result_buff, const char *pathname);
int RN(char *result_buff, const char *pathname, const char *pathname2);

int func_compare(const void *first, const void *second);
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: NON			                                             //
// 				                                                     //
// Purpose: Open socket												 //
//			accpet connect request from client						 //
// 	     	send ftp command by control connect stream				 //
// 			receive the result of ftp command by data connect stream //
///////////////////////////////////////////////////////////////////////
void main(int argc, char **argv)
{
	char *ptr;

	char result_buff[MAX_BUFF]; // buff for the result of ftp command
	char ctrl_buff[MAX_BUFF];	// buff for receiving from control connect server
	char data_buff[MAX_BUFF];	// buff for sending to data connect client

	char *host_ip;		   // data connect server ip
	unsigned int port_num; // data connect server port

	int ctrl_server_fd, ctrl_client_fd; // control connect
	struct sockaddr_in ctrl_server_addr, ctrl_client_addr;

	int data_server_fd; // data connect
	struct sockaddr_in data_server_addr;

	int clilen;

	/* your own codes */
	if (argc != 2)
	{
		ptr = "enter two arguments!";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return;
	}
	// open socket
	if ((ctrl_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return;
	}

	/* make control connection */
	////////// bind socket //////////
	memset(&ctrl_server_addr, 0, sizeof(ctrl_server_addr));
	ctrl_server_addr.sin_family = AF_INET;
	ctrl_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ctrl_server_addr.sin_port = htons(atoi(argv[1]));
	if (bind(ctrl_server_fd, (struct sockaddr *)&ctrl_server_addr, sizeof(ctrl_server_addr)) < 0)
	{
		ptr = "Server: Can't bind\n";
		write(STDERR_FILENO, ptr, strlen(ptr));
		return;
	}
	listen(ctrl_server_fd, 5);
	while (1)
	{
		clilen = sizeof(ctrl_client_addr);
		ctrl_client_fd = accept(ctrl_server_fd, (struct sockaddr *)&ctrl_client_addr, &clilen);

		while (1)
		{
			// memory initiating
			memset(ctrl_buff, 0, sizeof(ctrl_buff));
			memset(data_buff, 0, sizeof(data_buff));
			memset(result_buff, 0, sizeof(result_buff));

			read(ctrl_client_fd, ctrl_buff, MAX_BUFF); // receive ftp command form client
			printf("%s\n", ctrl_buff);
			if (!strcmp(ctrl_buff, "QUIT"))
			{ // if ftp command is "QUIT"
				ptr = "221 Goodbye.";
				write(ctrl_client_fd, ptr, strlen(ptr) + 1);
				printf("%s\n\n", ptr);
				break;
			}

			host_ip = convert_str_to_addr(ctrl_buff, &port_num); // seperate ip and port from PORT command
			/* make data connection */
			if ((data_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				ptr = "Server: Can't open stream socket.\n";
				write(STDERR_FILENO, ptr, strlen(ptr));
				exit(1);
			}
			/////////////// data connection to client /////////////////
			memset((char *)&data_server_addr, 0, sizeof(data_server_addr));
			data_server_addr.sin_family = AF_INET;
			data_server_addr.sin_addr.s_addr = inet_addr(host_ip);
			data_server_addr.sin_port = htons(port_num);
			free(host_ip);
			if (connect(data_server_fd, (struct sockaddr *)&data_server_addr, sizeof(data_server_addr)) < 0)
			{ // error
				ptr = "550 Failed to access.";
				write(ctrl_client_fd, ptr, strlen(ptr) + 1);
				printf("%s\n", ptr);
				continue;
			}
			else
			{ // connect success
				ptr = "200 Port command performed successfully.";
				write(ctrl_client_fd, ptr, strlen(ptr) + 1);
				printf("%s\n", ptr);
			}

			read(ctrl_client_fd, ctrl_buff, MAX_BUFF); // receive ftp command from client
			printf("%s\n", ctrl_buff);

			ptr = "150 Opening data connection for directory list.";
			write(ctrl_client_fd, ptr, strlen(ptr) + 1); // send code to client
			printf("%s\n", ptr);

			cmd_process(ctrl_buff, result_buff); // save the result of ftp command to result_buff
			if (write(data_server_fd, result_buff, MAX_BUFF) > 0)
			{ // sending result to client by data stream successes
				ptr = "226 Complete transmission.";
				write(ctrl_client_fd, ptr, strlen(ptr) + 1);
				printf("%s\n\n", ptr);
			}
			else
			{ // sending result to client by data stream fails
				ptr = "550 Failed transmission.";
				write(ctrl_client_fd, ptr, strlen(ptr) + 1);
				printf("%s\n\n", ptr);
			}
			close(data_server_fd); // close data stream
		}
		close(ctrl_client_fd); // close control stream
	}
}

///////////////////////////////////////////////////////////////////////
// convert_str_to_addr                                               //
// ================================================================= //
// Input: const char * -> PORT command				                 //
// 		  unsigned int * -> port	                                 //
// 				                                                     //
// Output: char * -> ip string										 //
// 				                                                     //
// Purpose: recieve PORT command									 //
// 	     	seperate ip, port from PORT command						 //
// 			save port number to unsigned int *port                   //
// 			return ip string pointer                                 //
///////////////////////////////////////////////////////////////////////
char *convert_str_to_addr(const char *str, unsigned int *port)
{ // client로부터 받은 PORT명령어에 붙은 IP주소와 포트번호를 변경
	char addr[30];
	char *ip;
	int j = 0;

	/* your converting algorithm */
	strcpy(addr, str + 5);
	ip = (char *)malloc(sizeof(char) * 30);
	j += sprintf(ip + j, "%s", strtok(addr, ","));
	for (int i = 0; i < 3; i++)
		j += sprintf(ip + j, ".%s", strtok(NULL, ","));

	*port = 0;
	*port += atoi(strtok(NULL, ",")) << 8;
	*port += atoi(strtok(NULL, ","));

	return ip;
}

///////////////////////////////////////////////////////////////////////
// cmd_process                                                       //
// ================================================================= //
// Input: const char * -> buff 						                 //
// 		  char * -> result_buffer                                    //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: recieve FTP command in buff								 //
//			execute FTP command										 //
// 	     Write the result of executing FTP command in result buffer  //
///////////////////////////////////////////////////////////////////////
int cmd_process(const char *buff, char *result_buff)
{
	int argc = 0;
	char *argv[100];
	char cp_buff[MAX_BUFF];
	char *ptr;
	int aflag = 0, lflag = 0, non_flag = 0, opflag = 0;
	int c;
	optind = 0; // reset optin, internal variable of getopt()

	memcpy(cp_buff, buff, MAX_BUFF);
	argv[argc++] = strtok(cp_buff, " ");
	while (ptr = strtok(NULL, " "))
	{
		argv[argc] = ptr;
		argc++;
	}
	argv[argc] = ptr;

	// arange in order [command - option - arguments]
	opterr = 0;
	while ((c = getopt(argc, argv, "al")) != -1)
	{
		switch (c)
		{
		case 'a':
			aflag++;
			break;
		case 'l':
			lflag++;
			break;
		case '?':
			non_flag++;
			break;
		}
	}

	//////////////// exception handling ////////////////////
	// no input exception
	if (!strcmp(argv[0], "NON"))
	{
		sprintf(result_buff, "Error: no input\n\n");
		return 0;
	}
	// no command exception
	if (!strcmp(argv[0], "WRONG"))
	{
		sprintf(result_buff, "Error: wrong command\n\n");
		return 0;
	}
	// invalid option exception
	if (non_flag > 0 || (strcmp(argv[0], "NLST") && aflag + lflag + non_flag > 0))
	{
		sprintf(result_buff, "Error: invalid option\n\n");
		return 0;
	}

	//////////////// command processing ////////////////////
	// NLST
	if (!strcmp(argv[0], "NLST"))
	{
		aflag > 0 ? opflag |= 2 : opflag; // aflag is LSB second bit
		lflag > 0 ? opflag |= 1 : opflag; // lflag is LSB first bit

		////////// Error handling ///////////////
		if (argc - optind > 1)
		{
			sprintf(result_buff, "Error: too many arguments...\n\n");
			return 0;
		}
		else if (NLST(result_buff, argv[optind], opflag) < 0)
		{
			sprintf(result_buff, "Error: %s\n\n", strerror(errno));
			return -1;
		}
	}
	// LIST
	else if (!strncmp(argv[0], "LIST", 5))
	{
		// Error handling
		if (argc - optind > 1)
		{
			sprintf(result_buff, "Error: too many arguments\n\n");
			return 0;
		}
		else if (LIST(result_buff, argv[optind]) == -1)
		{
			sprintf(result_buff, "Error: %s\n\n", strerror(errno));
			return -1;
		}
	}
	// PWD
	else if (!strncmp(argv[0], "PWD", 4))
	{
		// Error handling
		if (argc - optind > 0)
		{
			sprintf(result_buff, "Error: argument is not required\n\n");
			return 0;
		}
		else if (PWD(result_buff) < -1)
			return -1;
	}

	// CWD
	else if (!strncmp(argv[0], "CWD", 4))
	{
		// Error handling
		if (argc - optind > 1)
		{
			sprintf(result_buff, "Error: too many arguments...\n\n");
			return 0;
		}
		else if (CWD(result_buff, argv[optind]) == -1)
			return -1;
	}

	// CDUP
	else if (!strncmp(argv[0], "CDUP", 5))
	{
		// Error handling
		if (argc - optind > 0)
		{
			sprintf(result_buff, "Error: too many arguments...\n\n");
			return 0;
		}
		else if (CDUP(result_buff) == -1)
			return -1;
	}

	// MKD
	else if (!strncmp(argv[0], "MKD", 4))
	{
		int j = 0;

		// Error handling
		if (argc - optind == 0)
		{
			j += sprintf(result_buff + j, "Error: argument is required\n\n");
			return 0;
		}

		for (int i = optind; argc - i > 0; i++)
			MKD(result_buff, argv[i]);

		strcat(result_buff, "\n");
	}

	// DELE
	else if (!strncmp(argv[0], "DELE", 5))
	{
		int j = 0;

		// Error handling
		if (argc - optind == 0)
		{
			j += sprintf(result_buff + j, "Error: argument is required\n\n");
			return 0;
		}

		for (int i = optind; argc - i > 0; i++)
			DELE(result_buff, argv[i]);

		strcat(result_buff, "\n");
	}

	// RMD
	else if (!strncmp(argv[0], "RMD", 4))
	{
		int j = 0;

		if (argc - optind == 0)
		{
			j += sprintf(result_buff + j, "Error: argument is required\n\n");
			return 0;
		}

		for (int i = optind; argc - i > 0; i++)
			RMD(result_buff, argv[i]);

		strcat(result_buff, "\n");
	}

	// RNFR & RNTO
	else if (!strncmp(argv[0], "RNFR", 5))
	{
		struct stat st;
		// Error handling
		if (argc - optind != 3)
		{
			sprintf(result_buff, "Error: two arguments are required\n\n");
			return 0;
		}
		if (stat(argv[3], &st) == 0 && !S_ISDIR(st.st_mode))
		{
			sprintf(result_buff, "Error: name to change already exists\n\n");
			return 0;
		}
		RN(result_buff, argv[1], argv[3]);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////
// NLST                                                              //
// ================================================================= //
// Input: char * -> result_buffer 					                 //
// 		  const char * -> path	                                     //
// 		  int -> flag which includes information of flags a, l       //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Acts like linux terminal commands 'ls'					 //
// 	     Write the information of files named path in result buffer  //
///////////////////////////////////////////////////////////////////////
int NLST(char *result_buff, const char *pathname, int opflag)
{
	DIR *dp;
	struct dirent *dirp;
	struct dirent *dirpp[MAX_BUFF];
	struct stat st; // stat of dirp
	int num = 0;	// number of dirp

	char pathname_buff[MAX_BUFF];
	char *name = NULL; // file name

	//// experiment /////
	struct passwd *userInfo;	  // User information
	struct group *groupInfo;	  // Group information
	struct tm *modification_time; // Time

	int i, j, k;

	memset(pathname_buff, 0, MAX_BUFF);

	// if there's no pathname argument, pathname is "."
	if (pathname == NULL)
		pathname = ".";

	if (stat(pathname, &st) < 0)
		return -1;

	// if not directory
	if (!S_ISDIR(st.st_mode))
		return -1;

	// control directory only
	// open directory
	if ((dp = opendir(pathname)) == NULL)
		return -1;

	// save dirent in array
	while ((dirp = readdir(dp)))
	{
		dirpp[num] = dirp;
		num++;
	}
	dirpp[num] = dirp;

	// rearrange dirp in ascending order
	qsort(dirpp, num, sizeof(struct dirent *), func_compare);

	// l option ON
	if (opflag & 1)
	{
		j = 0;
		for (i = 0; i < num; i++)
		{
			name = dirpp[i]->d_name;
			if (strcmp(pathname, "."))
			{
				strcpy(pathname_buff, pathname);
				strcat(pathname_buff, "/");
				strcat(pathname_buff, name);
			}
			else
				strcpy(pathname_buff, name);

			////////// a option OFF //////////
			if (!(opflag & 2) && name[0] == '.')
				continue;

			if (stat(pathname_buff, &st) < 0)
				continue;
			if ((userInfo = getpwuid(st.st_uid)) == NULL)
				continue;
			if ((groupInfo = getgrgid(st.st_gid)) == NULL)
				continue;
			modification_time = localtime(&st.st_mtime);
			j += sprintf(result_buff + j, "%c", ((S_ISDIR(st.st_mode)) ? 'd' : '-'));
			j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRUSR) ? 'r' : '-', (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-');
			j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRGRP) ? 'r' : '-', (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-');
			j += sprintf(result_buff + j, "%c%c%c ", (st.st_mode & S_IROTH) ? 'r' : '-', (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
			j += sprintf(result_buff + j, "%3ld\t%s\t%s\t%5ld\t", st.st_nlink, userInfo->pw_name, groupInfo->gr_name, st.st_size);
			j += strftime(result_buff + j, sizeof(result_buff) - j, "%b %d %H:%M ", modification_time);
			j += sprintf(result_buff + j, "%s", name);
			if (S_ISDIR(st.st_mode))
				j += sprintf(result_buff + j, "/");
			j += sprintf(result_buff + j, "\n");
		}
	}
	// l option OFF
	else
	{
		k = 0;
		j = 0;
		for (i = 0; i < num; i++)
		{
			name = dirpp[i]->d_name;
			if (strcmp(pathname, "."))
			{
				strcpy(pathname_buff, pathname);
				strcat(pathname_buff, "/");
				strcat(pathname_buff, name);
			}
			else
				strcpy(pathname_buff, name);

			// aflag OFF
			if (!(opflag & 2) && name[0] == '.')
				continue;

			if (stat(pathname_buff, &st) < 0)
				continue;

			j += sprintf(result_buff + j, "%s", name);
			if (S_ISDIR(st.st_mode))
				j += sprintf(result_buff + j, "/");
			j += sprintf(result_buff + j, "\n");
		}
	}

	// close directory
	if (closedir(dp) < 0)
		return -1;

	return 0; // success 0, error -1
}

///////////////////////////////////////////////////////////////////////
// LIST                                                              //
// ================================================================= //
// Input: char * -> result_buff			                     		 //
// 		  const char * -> pathname                                   //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Operate the same as NLST -al                             //
//			argument should be directory name only                   //
///////////////////////////////////////////////////////////////////////
int LIST(char *result_buff, const char *pathname)
{
	DIR *dp;
	struct dirent *dirp;
	struct dirent *dirpp[MAX_BUFF];
	int num = 0; // number of dirp
	char name_buff[MAX_BUFF];
	struct stat st;				  // stat of dirp//
	struct passwd *userInfo;	  // User information
	struct group *groupInfo;	  // Group information
	struct tm *modification_time; // Time
	char *name = NULL;			  // file name
	int i, j, k;

	memset(name_buff, 0, MAX_BUFF);

	// if there's no pathname argument, pathname is "."
	if (pathname == NULL)
		pathname = ".";

	if (stat(pathname, &st) < 0)
		return -1;

	// if not directory
	if (!S_ISDIR(st.st_mode))
		return -1;

	// open directory
	if ((dp = opendir(pathname)) == NULL)
		return -1;

	// save dirent in array
	while (dirp = readdir(dp))
	{
		dirpp[num] = dirp;
		num++;
	}

	// rearrange dirp in ascending order
	qsort(dirpp, num, sizeof(struct dirent *), func_compare);

	// l option
	j = 0;
	for (i = 0; i < num; i++)
	{
		name = dirpp[i]->d_name;
		if (strcmp(pathname, "."))
		{
			strcpy(name_buff, pathname);
			strcat(name_buff, "/");
			strcat(name_buff, name);
		}
		else
			strcpy(name_buff, name);

		if (stat(name_buff, &st) < 0)
			continue;
		if ((userInfo = getpwuid(st.st_uid)) == NULL)
			continue;
		if ((groupInfo = getgrgid(st.st_gid)) == NULL)
			continue;
		modification_time = localtime(&st.st_mtime);
		j += sprintf(result_buff + j, "%c", ((S_ISDIR(st.st_mode)) ? 'd' : '-'));
		j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRUSR) ? 'r' : '-', (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-');
		j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRGRP) ? 'r' : '-', (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-');
		j += sprintf(result_buff + j, "%c%c%c ", (st.st_mode & S_IROTH) ? 'r' : '-', (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
		j += sprintf(result_buff + j, "%3ld\t%s\t%s\t%5ld\t", st.st_nlink, userInfo->pw_name, groupInfo->gr_name, st.st_size);
		j += strftime(result_buff + j, sizeof(result_buff) - j, "%b %d %H:%M ", modification_time);
		j += sprintf(result_buff + j, "%s", name);
		if (S_ISDIR(st.st_mode))
			j += sprintf(result_buff + j, "/");
		j += sprintf(result_buff + j, "\n");
	}

	// close directory
	if (closedir(dp) < 0)
		return -1;

	return 0; // success 0, error -1
}

///////////////////////////////////////////////////////////////////////
// PWD                                                               //
// ================================================================= //
// Input: char * -> result_buff			                 		     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Write path of current working directory in result_buff   //
///////////////////////////////////////////////////////////////////////
int PWD(char *result_buff)
{
	char wd[MAX_BUFF];

	if (getcwd(wd, MAX_BUFF) == NULL)
	{
		sprintf(result_buff, "Error: %s\n\n", strerror(errno));
		return -1;
	}
	else
	{
		sprintf(result_buff, "\"%s\" is current directory\n\n", wd);
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////
// CWD                                                               //
// ================================================================= //
// Input: char * -> result_buff										 //
//		  const char * -> pathname				                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Change location to path                			         //
// 			Write path of current working directory in result_buff   //
///////////////////////////////////////////////////////////////////////
int CWD(char *result_buff, const char *pathname)
{
	char wd[MAX_BUFF];
	int j = 0;

	if (chdir(pathname) < 0 || getcwd(wd, MAX_BUFF) == NULL)
	{
		sprintf(result_buff, "Error: %s\n\n", strerror(errno));
		return -1;
	}
	else
	{
		// print command
		j += sprintf(result_buff + j, "CWD %s\n", pathname);
		j += sprintf(result_buff + j, "\"%s\" is current directory\n\n", wd);

		return 0;
	}
}

///////////////////////////////////////////////////////////////////////
// CDUP                                                              //
// ================================================================= //
// Input: char * -> result_buff					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Change location to Parent directory   			         //
// 			Write path of current working directory in result_buff   //
///////////////////////////////////////////////////////////////////////
int CDUP(char *result_buff)
{
	char wd[MAX_BUFF];
	int j = 0;

	if (chdir("..") < 0 || getcwd(wd, MAX_BUFF) == NULL)
	{
		sprintf(result_buff, "Error: %s\n\n", strerror(errno));
		return -1;
	}
	else
	{
		// print command
		j += sprintf(result_buff + j, "CDUP\n");
		j += sprintf(result_buff + j, "\"%s\" is current directory\n\n", wd);

		return 0;
	}
}

///////////////////////////////////////////////////////////////////////
// MKD                                                               //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: just for using function mkdir()    			    		 //
// 			Write the result of function in result_buff   			 //
///////////////////////////////////////////////////////////////////////
int MKD(char *result_buff, const char *pathname)
{
	char str[MAX_BUFF];

	if (mkdir(pathname, 0775) == 0)
	{
		sprintf(str, "MKD %s ", pathname);
		strcat(result_buff, str);
		strcat(result_buff, "\n");
		return 0;
	}
	else
	{
		// Error handling
		if (errno == EEXIST)
			sprintf(result_buff + strlen(result_buff), "Error: cannot create directory '%s': file exists\n", pathname);
		else
			sprintf(result_buff + strlen(result_buff), "Error: %s\n", strerror(errno));
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////
// DELE                                                              //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: just for using function unlink()    					 //
// 			Write the result of function in result_buff   			 //
///////////////////////////////////////////////////////////////////////
int DELE(char *result_buff, const char *pathname)
{
	char str[MAX_BUFF];

	if (unlink(pathname) == 0)
	{
		sprintf(str, "DELE %s ", pathname);
		strcat(result_buff, str);
		strcat(result_buff, "\n");
		return 0;
	}
	else
	{
		// Error handling
		if (errno == ENOENT)
			sprintf(result_buff + strlen(result_buff), "Error: failed to delete '%s' \n", pathname);
		else
			sprintf(result_buff + strlen(result_buff), "Error: %s\n", strerror(errno));

		return -1;
	}
}

///////////////////////////////////////////////////////////////////////
// RMD                                                               //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: just for using function rmdir()    					     //
// 			Write the result of function in result_buff   			 //
///////////////////////////////////////////////////////////////////////
int RMD(char *result_buff, const char *pathname)
{
	char str[MAX_BUFF];

	if (rmdir(pathname) == 0)
	{
		sprintf(str, "RMD %s ", pathname);
		strcat(result_buff, str);
		strcat(result_buff, "\n");
		return 0;
	}
	else
	{
		// Error handling
		if (errno == ENOENT)
			sprintf(result_buff + strlen(result_buff), "Error: failed to remove '%s'\n", pathname);
		else
			sprintf(result_buff + strlen(result_buff), "Error: %s\n", strerror(errno));

		return -1;
	}
}

///////////////////////////////////////////////////////////////////////
// RN                                                                //
// ================================================================= //
// Input: const void * -> path1					                     //
// 	      const void * -> path2                                      //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: just for using function rename() 					     //
// 			Write the result of function in result_buff   			 //
///////////////////////////////////////////////////////////////////////
int RN(char *result_buff, const char *pathname1, const char *pathname2)
{

	if (rename(pathname1, pathname2) == 0)
	{
		sprintf(result_buff, "RNFR %s RNTO %s\n", pathname1, pathname2);
		return 0;
	}
	else
	{
		// Error handling
		sprintf(result_buff, "Error: %s\n\n", strerror(errno));
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////
// func_compare                                                      //
// ================================================================= //
// Input: const void * -> comparison target1					     //
// 	      const void * -> comparison target2                         //
// 				                                                     //
// Output: int - 0 same                     					     //
// 	           - < 0	target1 < target2                            //
// 	           - > 0	target1 > target2                  		     //
// 				                                                     //
// Purpose: used as argument in qsort function					     //
// 	        compare ASCII code of d_name of struct dirent	         //
///////////////////////////////////////////////////////////////////////
int func_compare(const void *first, const void *second)
{
	return strcmp((*(struct dirent **)first)->d_name, (*(struct dirent **)second)->d_name);
}