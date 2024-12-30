#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>

#define MAX_BUFF 4096

int client_info(struct sockaddr_in *cli_addr);
int cmd_process(const char *buff, char *result_buff);
int NLST(char *result_buff, const char *pathname, int opflag);
int func_compare(const void *first, const void *second);

///////////////////////////////////////////////////////////////////////
// main	                                                             //
// ================================================================= //
// Input: int -> the number of arguments			                 //
// 		  char ** -> the array of the pointer of each argument       //
// 				                                                     //
// Output: int ->	0 success                                        //
// 				   -1 fail                                           //
// 				                                                     //
// Purpose: recieve FTP command as arguments, 		 				 //
// 	     execute FTP command, send the result to Client				 //
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	struct sockaddr_in srv_addr, cli_addr;
	int srv_fd, cli_fd;
	char buff[MAX_BUFF], result_buff[MAX_BUFF];
	int len, len_out;
	char *ptr; // string pointer
	//////////// memory initialization /////////////
	memset((char *)&srv_addr, 0, sizeof(srv_addr));
	memset(buff, 0, MAX_BUFF);
	memset(result_buff, 0, MAX_BUFF);

	// input error
	if (argc < 2)
	{
		ptr = "enter port number\n";
		write(STDOUT_FILENO, ptr, strlen(ptr));
		return 0;
	}

	//////////// open socket //////////////////
	if ((srv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ptr = "Server: Can't open stream socket.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));

		return 0;
	}

	///////////// bind socket ////////////////////
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(atoi(argv[1]));
	if (bind(srv_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
	{
		ptr = "Server: Can't bind local address.\n";
		write(STDERR_FILENO, ptr, strlen(ptr));

		return 0;
	}

	listen(srv_fd, 5);
	while (1)
	{
		len = sizeof(cli_addr);
		cli_fd = accept(srv_fd, (struct sockaddr *)&cli_addr, &len);
		/* display client ip and port */
		if (client_info(&cli_addr) < 0)
		{
			ptr = "client_info() error!!\n";
			write(STDERR_FILENO, ptr, strlen(ptr));
		}

		while (len_out = read(cli_fd, buff, MAX_BUFF - 1) > 0)
		{
			/* command execute and result */
			if (cmd_process(buff, result_buff) < 0)
			{
				ptr = "cmd_process() error!!\n";
				write(STDERR_FILENO, ptr, strlen(ptr));
				break;
			}

			// write FTP command to terminal
			write(STDOUT_FILENO, buff, strlen(buff));
			write(STDOUT_FILENO, "\n", 1);

			// send result to client
			write(cli_fd, result_buff, strlen(result_buff));
			if (!strcmp(result_buff, "QUIT"))
				break;

			memset(buff, 0, MAX_BUFF);
			memset(result_buff, 0, MAX_BUFF);
		}
		close(cli_fd);
		ptr = "client disconnected\n";
		write(STDOUT_FILENO, ptr, strlen(ptr));
		memset(buff, 0, MAX_BUFF);
		memset(result_buff, 0, MAX_BUFF);
	}
	close(srv_fd);

	return 0;
}

int client_info(struct sockaddr_in *cli_addr)
{
	char buf[MAX_BUFF];
	int j = 0;

	j = sprintf(buf, "==========Client info==========\n");
	j += sprintf(buf + j, "client IP: %s\n\n", inet_ntoa(cli_addr->sin_addr));
	j += sprintf(buf + j, "client port: %d\n", ntohs((int)cli_addr->sin_port));
	j += sprintf(buf + j, "===============================\n");

	return write(STDOUT_FILENO, buf, strlen(buf));
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
// Purpose: recieve FTP command in buff, execute FTP command,		 //
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
	if (!strcmp(argv[0], "QUIT"))
	{
		strncpy(result_buff, "QUIT", 5);
		return 0;
	}
	else if (!strcmp(argv[0], "NLST"))
	{
		////////// Error handling ///////////////
		if (argc - optind > 1)
		{
			sprintf(result_buff, "Error: too many arguments...\n\n");
			return 0;
		}

		aflag > 0 ? opflag |= 2 : opflag; // aflag is LSB second bit
		lflag > 0 ? opflag |= 1 : opflag; // lflag is LSB first bit

		if (NLST(result_buff, argv[optind], opflag) < 0)
		{
			sprintf(result_buff, "Error: %s\n\n", strerror(errno));
			return -1;
		}

		return 0;
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

	// if pathname is filename
	if (S_ISREG(st.st_mode))
	{
		// l option OFF
		if (!(opflag & 1))
			sprintf(result_buff, "%s\n", pathname);

		// l option
		else
		{
			j = 0;
			if (stat(pathname, &st) < 0)
				return -1;
			if ((userInfo = getpwuid(st.st_uid)) == NULL)
				return -1;
			if ((groupInfo = getgrgid(st.st_gid)) == NULL)
				return -1;

			modification_time = localtime(&st.st_mtime);
			j += sprintf(result_buff + j, "%c", ((S_ISDIR(st.st_mode)) ? 'd' : '-'));
			j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRUSR) ? 'r' : '-', (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-');
			j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRGRP) ? 'r' : '-', (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-');
			j += sprintf(result_buff + j, "%c%c%c ", (st.st_mode & S_IROTH) ? 'r' : '-', (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
			j += sprintf(result_buff + j, "%3ld\t%s\t%s\t%5ld\t", st.st_nlink, userInfo->pw_name, groupInfo->gr_name, st.st_size);
			j += strftime(result_buff + j, sizeof(result_buff) - j, "%b %d %H:%M ", modification_time);
			j += sprintf(result_buff + j, "%s", pathname);
			j += sprintf(result_buff + j, "\n");
		}

		return 0;
	}

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
// func_compare                                                      //
// ================================================================= //
// Input: const void * -> comparison target1					     //
// 	      const void * -> comparison target2                         //
// 				                                                     //
// Output: int - 0 same                     					     //
// 	           - < 0 target1 < target2                               //
// 	           - > 0 target1 > target2                  		     //
// 				                                                     //
// Purpose: used as argument in qsort function					     //
// 	        compare ASCII code of d_name of struct dirent	         //
///////////////////////////////////////////////////////////////////////
int func_compare(const void *first, const void *second)
{
	return strcmp((*(struct dirent **)first)->d_name, (*(struct dirent **)second)->d_name);
}