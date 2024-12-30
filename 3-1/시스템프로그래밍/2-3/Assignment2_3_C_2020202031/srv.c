///////////////////////////////////////////////////////////////////////
// File Name : srv.c		                                         //
// Date : 2024/05/16                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim Jae Hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #2-3 ( ftp server )          //
// Description : Learn and practice                                  //
//               Socket Programming & process handling               //
//				 in side of server									 //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>

#define MAX_BUFF 4096

/////// declaration realated  to INFO ////////
typedef struct info
{
	pid_t pid;
	int port;
	time_t start;
	struct info *next;
} INFO;

INFO *head = NULL; // linked list
void INFO_init();
void INFO_add(pid_t pid, int port, time_t start);
int INFO_remove(pid_t pid);
////////////////////////////////////////////////

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

int num_children = 0; // the number of child processes

void sh_chld(int);								  // signal handler for SIGCHLD
void sh_alrm(int);								  // signal handler for SIGALRM
void sh_int(int);								  // signal handler for SIGINT
void print_cli_info(struct sockaddr_in cli_addr); // print client info
void print_chd_pid(int pid);					  // print child's pid
void print_processes_info();					  // print child processes information

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: int -> 0 		                                         //
// 				                                                     //
// Purpose: recieve FTP command from user							 //
//			processing FTP command									 //
//			sent the result to the clients							 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	char buff[MAX_BUFF], result_buff[MAX_BUFF];
	int n;
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	int len;
	INFO *cur;

	signal(SIGCHLD, sh_chld);
	signal(SIGALRM, sh_alrm);
	signal(SIGINT, sh_int);
	alarm(10);

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
		num_children++;
		if (pid > 0) // parent
		{
			INFO_add(pid, ntohs(client_addr.sin_port), time(NULL));
			print_cli_info(client_addr); // print client information(ip, port)
			print_chd_pid(pid);			 // print child's process id
			print_processes_info();
			alarm(10);
		}
		else if (pid == 0) // child
		{
			while (1)
			{
				memset(buff, 0, MAX_BUFF);
				memset(result_buff, 0, MAX_BUFF);

				read(client_fd, buff, MAX_BUFF); // read from Client
				if (!strcmp(buff, "QUIT"))		 // if string is "QUIT"
				{
					INFO_init();
					exit(1);
				}
				else
				{
					cmd_process(buff, result_buff);
					printf("> %s\t\t[%d]\n", buff, getpid());
					write(client_fd, result_buff, MAX_BUFF); // send string as is to Client
				}
			}
		}

		close(client_fd); // close client socket
	}

	INFO_init();
	close(server_fd); // close server socket

	return 0;
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
			j += (++k % 5 != 0) ? sprintf(result_buff + j, "\t") : sprintf(result_buff + j, "\n");
		}
		strcat(result_buff, "\n");
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

///////////////////////////////////////////////////////////////////////
// sh_chld	                                                         //
// ================================================================= //
// Input: int -> signum											     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: when child's status changed(terminated)				     //
// 	        get child's terminate status and pid			         //
///////////////////////////////////////////////////////////////////////
void sh_chld(int signum)
{
	pid_t pid;

	while ((pid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("Client( %d)'s Release.\n\n", pid);
		INFO_remove(pid);
		num_children--;
	}
}

///////////////////////////////////////////////////////////////////////
// sh_alrm	                                                         //
// ================================================================= //
// Input: int -> signum											     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: when alrm occurs,									     //
// 	        print children processes information			         //
//			set alarm 10secs										 //
///////////////////////////////////////////////////////////////////////
void sh_alrm(int signum)
{
	print_processes_info();
	alarm(10);
}

///////////////////////////////////////////////////////////////////////
// sh_int	                                                         //
// ================================================================= //
// Input: int -> signum											     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: when interrupt occurs,									 //
// 	        get zombie processes terminate status and pid	         //
//			terminate process										 //
///////////////////////////////////////////////////////////////////////
void sh_int(int signum)
{
	pid_t pid;

	while ((pid = wait(NULL)) != -1)
	{
	}

	INFO_init();
	exit(1);
}

///////////////////////////////////////////////////////////////////////
// print_cli_info                                                    //
// ================================================================= //
// Input: struct sockaddr_in -> client sockaddr					     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: print client's information								 //
///////////////////////////////////////////////////////////////////////
void print_cli_info(struct sockaddr_in cli_addr)
{
	char buf[MAX_BUFF];
	int j = 0;

	j = sprintf(buf, "==========Client info==========\n");
	j += sprintf(buf + j, "client IP: %s\n\n", inet_ntoa(cli_addr.sin_addr));
	j += sprintf(buf + j, "client port: %d\n", ntohs((int)cli_addr.sin_port));
	j += sprintf(buf + j, "===============================\n");

	write(STDOUT_FILENO, buf, strlen(buf));
}

///////////////////////////////////////////////////////////////////////
// print_chd_pid                                                     //
// ================================================================= //
// Input: int -> pid											     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: print child's process id								 //
///////////////////////////////////////////////////////////////////////
void print_chd_pid(int pid)
{
	char buf[MAX_BUFF];

	sprintf(buf, "Child Process ID : %d\n", pid);
	write(STDOUT_FILENO, buf, strlen(buf));
}

///////////////////////////////////////////////////////////////////////
// print_processes_info                                              //
// ================================================================= //
// Input: NON													     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: print children processes' information					 //
// 			information: PID, PORT, TIME							 //
///////////////////////////////////////////////////////////////////////
void print_processes_info()
{
	INFO *cur = head;
	time_t interval;
	struct tm *gt;

	printf("\nCurrent Number of Client :  %d\n", num_children);
	printf("  PID\t PORT\t TIME\n");

	while (cur)
	{
		// interval = time(NULL) - cur->start;
		// gt = gmtime(&interval);
		printf("%5d\t%5d\t%5ld\n", cur->pid, cur->port, time(NULL) - cur->start + 1);
		cur = cur->next;
	}
	printf("\n");
}

///////////////////////////////////////////////////////////////////////
// INFO_init		                                            	 //
// ================================================================= //
// Input: NON													     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: linked list initialization								 //
///////////////////////////////////////////////////////////////////////
void INFO_init()
{
	INFO *cur = head;

	if (head == NULL)
	{
		return;
	}
	else
	{
		while (cur != NULL)
		{
			head = cur->next;
			free(cur);
			cur = head;
		}
	}
}

///////////////////////////////////////////////////////////////////////
// INFO_add			                                            	 //
// ================================================================= //
// Input: NON													     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: linked list adding										 //
///////////////////////////////////////////////////////////////////////
void INFO_add(pid_t pid, int port, time_t start)
{
	INFO *cur = head;
	INFO *new_node = (INFO *)malloc(sizeof(INFO));
	new_node->pid = pid;
	new_node->port = port;
	new_node->start = time(NULL);
	new_node->next = NULL;

	// Check first element
	if (head == NULL)
	{
		head = new_node;
	}
	else
	{
		// Add new node
		while (cur->next)
			cur = cur->next;
		cur->next = new_node;
	}
}

///////////////////////////////////////////////////////////////////////
// INFO_remove		                                            	 //
// ================================================================= //
// Input: NON													     //
// 				                                                     //
// Output: NON			                     					     //
// 				                                                     //
// Purpose: linked list removing									 //
///////////////////////////////////////////////////////////////////////
int INFO_remove(pid_t pid)
{
	INFO *cur;
	INFO *prev;

	if (head == NULL)
	{
		return 0;
	}

	if (head->pid == pid)
	{
		cur = head;
		head = head->next;
		free(cur);
		return 1;
	}
	else
	{
		prev = head;
		cur = head->next;
		while (cur != NULL && cur->pid != pid)
		{
			prev = cur;
			cur = cur->next;
		}

		if (cur == NULL)
			return 0;

		prev->next = cur->next;
		free(cur);
		return 1;
	}
}