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

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <netinet/in.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_BUFF 1024
#define MAX_DBUFF 16000

#define FLAGS (O_RDWR | O_CREAT | O_TRUNC)
#define ASCII_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define BIN_MODE (S_IXUSR | S_IXGRP | S_IXOTH)

int ctrl_server_fd, ctrl_client_fd; // control connect
struct sockaddr_in ctrl_server_addr, ctrl_client_addr;

int data_server_fd; // data connect
struct sockaddr_in data_server_addr;

int clilen;

mode_t f_mode = BIN_MODE; // file open mode
char log_buff[MAX_BUFF];  // for writing to logfile
char time_buff[MAX_BUFF]; // for strftime
char user[MAX_BUFF];      // User logging in

int log_fd; // log file fd
time_t t;
struct tm *timeinfo;

////////////////// user login //////////////////////////
int check_ip(const char *userIP);
int log_auth(int connfd);
int userID_match(char *user);
int user_match(char *user, char *passwd);
//////////////////////////////////////////////////////
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
int RETR(char *result_buff, const char *filename);
int STOR(char *result_buff, const char *filename);

int remove_r(char *buff, int buff_len);
int func_compare(const void *first, const void *second);
//////////////////////////////////////////////////////

void write_log(const char *str);
void write_log_start_end(const char *str);
void sh_int(int signum);

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

    char buff[MAX_BUFF]; // used in everywhere. just temp buff
    char ftp_buff[MAX_BUFF];
    char ctrl_buff[MAX_BUFF];  // buff for receiving from control connect server
    char data_buff[MAX_DBUFF]; // buff for sending to data connect client

    char *host_ip;         // data connect server ip
    unsigned int port_num; // data connect server port

    int j;   // for sprintf
    int len; // buff len
    time_t t;
    struct tm *timeinfo;
    char log_buff[100]; // for writing to logfile

    pid_t p;

    signal(SIGINT, sh_int);
    log_fd = open("logfile", O_RDWR | O_CREAT | O_APPEND, ASCII_MODE);

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

    ///////////// initailization buffs /////////////////////
    memset(ftp_buff, 0, sizeof(ftp_buff));
    memset(ctrl_buff, 0, sizeof(ctrl_buff));
    memset(data_buff, 0, sizeof(data_buff));

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

    write_log_start_end("Server is started");
    // server accept new client repeatedly
    while (1)
    {
        // memory initiating
        memset(ftp_buff, 0, sizeof(ftp_buff));
        memset(ctrl_buff, 0, sizeof(ctrl_buff));
        memset(data_buff, 0, sizeof(data_buff));

        clilen = sizeof(ctrl_client_addr);
        ctrl_client_fd = accept(ctrl_server_fd, (struct sockaddr *)&ctrl_client_addr, &clilen);

        p = fork();
        if (p == 0)
        { /* Client의 IP가 접근 가능한지 확인 */
            if (check_ip(inet_ntoa(ctrl_client_addr.sin_addr)) == 0)
            {
                strcpy(ctrl_buff, "431 This client can't access. Close the session.\n");
                write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));

                close(ctrl_client_fd);
                continue;
            }

            int motd_fd;
            motd_fd = open("motd", O_RDONLY);
            read(motd_fd, buff, sizeof(buff));
            time(&t);
            timeinfo = localtime(&t);
            strftime(time_buff, sizeof(time_buff), "%a %b %d %H:%M:%S %Y", timeinfo);
            sprintf(ctrl_buff, buff, time_buff);
            strcat(ctrl_buff, "\n");
            write(ctrl_client_fd, ctrl_buff,
                  MAX_BUFF); // 220 sswlab.kw.ac.kr FTP server (version myftp [1.0] date) ready.
            write(STDOUT_FILENO, ctrl_buff,
                  MAX_BUFF); // 220 sswlab.kw.ac.kr FTP server (version myftp [1.0] date) ready.

            if (log_auth(ctrl_client_fd) == 0)
            {
                // if 3 times fail (ok : 1, fail : 0)
                close(ctrl_client_fd);
                continue;
            }

            ///////////// sending and receiving start ///////////////////
            while (1)
            {
                // memory initiating
                memset(ftp_buff, 0, sizeof(ftp_buff));
                memset(ctrl_buff, 0, sizeof(ctrl_buff));
                memset(data_buff, 0, sizeof(data_buff));

                read(ctrl_client_fd, ctrl_buff, MAX_BUFF); // receive command form client
                write_log(ctrl_buff);

                //////////////////// QUIT ///////////////////////
                if (!strcmp(ctrl_buff, "QUIT"))
                {
                    strcpy(ctrl_buff, "221 Goodbye.\n");
                    write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                    write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                    write_log(ctrl_buff);
                    exit(1);
                }
                //////////////////// NLST, LIST, RETR, STOR ///////////////////////
                else if (!strncmp(ctrl_buff, "PORT", 4))
                {
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
                        strcpy(ctrl_buff, "550 Failed to access.\n");
                        write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                        write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        write_log(ctrl_buff);
                        continue;
                    }
                    else
                    { // connect success
                        strcpy(ctrl_buff, "200 Port command performed successfully.\n");
                        write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                        write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        write_log(ctrl_buff);
                    }

                    read(ctrl_client_fd, ftp_buff, MAX_BUFF); // receive ftp command form client
                    write_log(ctrl_buff);

                    //////////////////////// NLST LIST /////////////////////////////
                    if (!strncmp(ftp_buff, "NLST", 4) || !strncmp(ftp_buff, "LIST", 4))
                    {
                        strcpy(ctrl_buff, "150 Opening data connection for directory list.\n");
                        write(ctrl_client_fd, ctrl_buff, MAX_BUFF); // send code to client
                        write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        write_log(ctrl_buff);

                        len = cmd_process(ftp_buff, data_buff); // save the result of ftp command to result_buff
                        if (len == -1)
                        {
                            write(ctrl_client_fd, "not exist", MAX_BUFF);
                            strcpy(ctrl_buff, "550 Failed transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }
                        else
                        { // sending file details to client by data stream
                            write(ctrl_client_fd, "exist", MAX_BUFF);

                            write(data_server_fd, data_buff, strlen(data_buff)); // file data
                            strcpy(ctrl_buff, "226 Complete transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }
                        write_log(ctrl_buff);
                    }

                    //////////////////////// RETR /////////////////////////////
                    else if (!strncmp(ftp_buff, "RETR", 4))
                    {
                        if (f_mode == ASCII_MODE)
                            sprintf(ctrl_buff, "150 Opening ASCII mode data connection for %s.\n", ftp_buff + 5);
                        else
                            sprintf(ctrl_buff, "150 Opening BINARY mode data connection for %s.\n", ftp_buff + 5);
                        write(ctrl_client_fd, ctrl_buff, MAX_BUFF); // send code to client
                        write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        write_log(ctrl_buff);

                        len = cmd_process(ftp_buff, data_buff); // save the result of ftp command to result_buff
                        if (len == -1)
                        {
                            write(ctrl_client_fd, "not exist", MAX_BUFF);
                            strcpy(ctrl_buff, "550 Failed transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }
                        else
                        { // sending file details to client by data stream
                            write(ctrl_client_fd, "exist", MAX_BUFF);
                            write(ctrl_client_fd, ftp_buff + 5, MAX_BUFF); // file name

                            write(data_server_fd, data_buff, len); // file data
                            strcpy(ctrl_buff, "226 Complete transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }

                        write_log(ctrl_buff);
                    }

                    //////////////////////// STOR /////////////////////////////
                    else
                    { // ftp command is "STOR"
                        if (f_mode == ASCII_MODE)
                            sprintf(ctrl_buff, "150 Opening ASCII mode data connection for %s.\n", ftp_buff + 5);
                        else
                            sprintf(ctrl_buff, "150 Opening BINARY mode data connection for %s.\n", ftp_buff + 5);
                        write(ctrl_client_fd, ctrl_buff, MAX_BUFF); // send code to client
                        write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        write_log(ctrl_buff);

                        read(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                        if (!strcmp(ctrl_buff, "not exist"))
                            continue;

                        len = cmd_process(ftp_buff, data_buff); // save the result of ftp command to result_buff
                        if (len == -1)
                        { // sending nothing to client by data stream
                            write(ctrl_client_fd, "exist", MAX_BUFF);

                            strcpy(ctrl_buff, "550 Failed transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }
                        else
                        { // sending file details to client by data stream
                            strcpy(ctrl_buff, "226 Complete transmission.\n");
                            write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                            write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                        }

                        write_log(ctrl_buff);
                    }
                    close(data_server_fd); // close data stream
                }
                // USER input command which doesn't need data connection
                else
                {
                    strcpy(ftp_buff, ctrl_buff);
                    cmd_process(ftp_buff, ctrl_buff); // save the result of ftp command to ctrl_buff
                    write(ctrl_client_fd, ctrl_buff, MAX_BUFF);
                    write(STDOUT_FILENO, ctrl_buff, strlen(ctrl_buff));
                    write_log(ctrl_buff);
                }
            }
            close(ctrl_client_fd); // close control stream
            exit(1);
        }
    }
    close(log_fd);
    close(log_fd);
}

///////////////////////////////////////////////////////////////////////
// cmd_process                                                       //
// ================================================================= //
// Input: const char * -> buff 						                 //
// 		  char * -> result_buffer                                    //
// 				                                                     //
// Output: int -  0 seccess                                          //
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
        sprintf(result_buff, "Error: no input\n");
        return -1;
    }
    // no command exception
    if (!strcmp(argv[0], "WRONG"))
    {
        sprintf(result_buff, "Error: wrong command\n");
        return -1;
    }
    // invalid option exception
    if (non_flag > 0 || (strcmp(argv[0], "NLST") && aflag + lflag + non_flag > 0))
    {
        sprintf(result_buff, "Error: invalid option\n");
        return -1;
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
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }
        else if (NLST(result_buff, argv[optind], opflag) < 0)
        {
            sprintf(result_buff, "Error: %s\n", strerror(errno));
            return -1;
        }
    }
    // LIST
    else if (!strcmp(argv[0], "LIST"))
    {
        // Error handling
        if (argc - optind > 1)
        {
            sprintf(result_buff, "Error: too many arguments\n");
            return -1;
        }
        else if (LIST(result_buff, argv[optind]) == -1)
        {
            sprintf(result_buff, "Error: %s\n", strerror(errno));
            return -1;
        }
    }
    // PWD
    else if (!strcmp(argv[0], "PWD"))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(result_buff, "Error: argument is not required\n");
            return -1;
        }
        if (PWD(result_buff) == -1)
            return -1;
    }

    // CWD
    else if (!strcmp(argv[0], "CWD"))
    {
        // Error handling
        if (argc - optind > 1)
        {
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }
        else if (CWD(result_buff, argv[optind]) == -1)
            return -1;
    }

    // CDUP
    else if (!strcmp(argv[0], "CDUP"))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }
        else if (CDUP(result_buff) == -1)
            return -1;
    }

    // MKD
    else if (!strcmp(argv[0], "MKD"))
    {
        int j = 0;

        // Error handling
        if (argc - optind == 0)
        {
            sprintf(result_buff, "Error: argument is required\n");
            return -1;
        }

        for (int i = optind; argc - i > 0; i++)
            if (MKD(result_buff, argv[i]) == -1)
                return -1;
    }

    // DELE
    else if (!strcmp(argv[0], "DELE"))
    {
        int j = 0;

        // Error handling
        if (argc - optind == 0)
        {
            sprintf(result_buff, "Error: argument is required\n");
            return -1;
        }

        for (int i = optind; argc - i > 0; i++)
            if (DELE(result_buff, argv[i]) == -1)
                return -1;
    }

    // RMD
    else if (!strcmp(argv[0], "RMD"))
    {
        int j = 0;

        if (argc - optind == 0)
        {
            sprintf(result_buff, "Error: argument is required\n");
            return -1;
        }

        for (int i = optind; argc - i > 0; i++)
            if (RMD(result_buff, argv[i]) == -1)
                return -1;
    }

    // RNFR & RNTO
    else if (!strcmp(argv[0], "RNFR"))
    {
        struct stat st;
        // Error handling
        if (argc - optind != 3)
        {
            sprintf(result_buff, "Error: two arguments are required\n");
            return -1;
        }
        if (stat(argv[3], &st) == 0 && !S_ISDIR(st.st_mode))
        {
            sprintf(result_buff, "Error: name to change already exists\n");
            return -1;
        }
        if (RN(result_buff, argv[1], argv[3]) == -1)
            return -1;
    } // RETR
    else if (!strcmp(argv[0], "RETR"))
    {
        // Error handling
        if (argc - optind > 1)
        {
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }

        return RETR(result_buff, argv[1]);
    } // STOR
    else if (!strcmp(argv[0], "STOR"))
    {
        // Error handling
        if (argc - optind > 1)
        {
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }
        return STOR(result_buff, argv[1]);
    } // TYPE
    else if (!strcmp(argv[0], "TYPE"))
    {
        if (!strcmp(argv[1], "I"))
        {
            f_mode = BIN_MODE;
            strcpy(result_buff, "TYPE I \n");
        }
        else
        {
            f_mode = ASCII_MODE;
            strcpy(result_buff, "TYPE A \n");
        }
    }
    // QUIT
    else if (!strcmp(argv[0], "QUIT"))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(result_buff, "Error: too many arguments...\n");
            return -1;
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////
// NLST                                                              //
// ================================================================= //
// Input: char * -> result_buffer 					                 //
// 		  const char * -> path	                                     //
// 		  int -> flag which includes information of flags a, l       //
// 		  l : 1, a : 2, al : 3                                       //
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
    int num = 0;    // number of dirp

    char pathname_buff[MAX_BUFF];
    char *name = NULL; // file name

    //// experiment /////
    struct passwd *userInfo;      // User information
    struct group *groupInfo;      // Group information
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
            j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRUSR) ? 'r' : '-',
                         (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-');
            j += sprintf(result_buff + j, "%c%c%c", (st.st_mode & S_IRGRP) ? 'r' : '-',
                         (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-');
            j += sprintf(result_buff + j, "%c%c%c ", (st.st_mode & S_IROTH) ? 'r' : '-',
                         (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
            j += sprintf(result_buff + j, "%3ld\t%s\t%s\t%5ld\t", st.st_nlink, userInfo->pw_name, groupInfo->gr_name,
                         st.st_size);
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
    return NLST(result_buff, pathname, 3);
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
        sprintf(result_buff, "Error: %s", strerror(errno));
        return -1;
    }
    else
    {
        sprintf(result_buff, "257 \"%s\" is current directory.\n", wd);
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
        sprintf(result_buff, "550 %s: Can't find such file or directory.\n", pathname);
        return -1;
    }
    else
    {
        strcpy(result_buff, "250 CWD command performed successfully.\n");
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
    if (chdir("..") < 0)
    {
        strcpy(result_buff, "550 ..: Can't find such file or directory.\n");
        return -1;
    }
    else
    {
        strcpy(result_buff, "250 CDUP command performed successfully.\n");
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

    if (mkdir(pathname, 0775) == 0)
    {
        strcpy(result_buff, "250 MKD command performed successfully.\n");
        return 0;
    }
    else
    {
        // Error handling
        sprintf(result_buff, "550 %s: Can't create directory.\n", pathname);
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
    if (unlink(pathname) == 0)
    {
        strcpy(result_buff, "250 DELE command performed successfully.\n");
        return 0;
    }
    else
    {
        // Error handling
        sprintf(result_buff, "550 %s: Can't find such file or directory.\n", pathname);
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
    if (rmdir(pathname) == 0)
    {
        strcpy(result_buff, "250 RMD command performed successfully.\n");
        return 0;
    }
    else
    {
        // Error handling
        sprintf(result_buff, "550 %s: Can't remove directory.\n", pathname);
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
        sprintf(result_buff, "250 RNFR %s RNTO %s\n", pathname1, pathname2);
        return 0;
    }
    else
    {
        // Error handling
        sprintf(result_buff, "550 Error: %s\n\n", strerror(errno));
        return -1;
    }
}

///////////////////////////////////////////////////////////////////////
// RETR                                                              //
// ================================================================= //
// Input: char * -> 	buff					                     //
// 		  const char * -> file name                                  //
// 				                                                     //
// Output: int -  length of buff                                     //
// 				  -1 on fail                                         //
// 				                                                     //
// Purpose: read file and store data to buff						 //
//			to send file data from server 					   	     //
// 														   			 //
///////////////////////////////////////////////////////////////////////
int RETR(char *buff, const char *filename)
{
    int fp;
    int len = 0;

    if (access(filename, R_OK) < 0)
        return -1;

    fp = open(filename, O_RDONLY);
    while (len = read(fp, buff, MAX_BUFF))
    {
        if (len != -1)
            break;
    }
    close(fp);

    if (f_mode == ASCII_MODE)
        len = remove_r(buff, len);

    return len;
}

///////////////////////////////////////////////////////////////////////
// STOR                                                              //
// ================================================================= //
// Input: const char * -> file name                                  //
// 				                                                     //
// Output: int -  length of buff                                     //
// 				  -1 on fail                                         //
// 				                                                     //
// Purpose: receive file data										 //
//			and create file in server directory				   	     //
///////////////////////////////////////////////////////////////////////
int STOR(char *result_buff, const char *filename)
{
    int fp;
    int len = 0;

    if (access(filename, R_OK) == 0)
        return -1;

    write(ctrl_client_fd, "not exist", MAX_BUFF);

    len = read(data_server_fd, result_buff, MAX_DBUFF);
    if (f_mode == ASCII_MODE)
        len = remove_r(result_buff, len);

    fp = open(filename, FLAGS, f_mode);
    write(fp, result_buff, len);

    close(fp);

    return len;
}

///////////////////////////////////////////////////////////////////////
// remove_r                                                          //
// ================================================================= //
// Input: char * -> buff						                     //
// 				                                                     //
// Output: int -> length after removing '\r'                         //
// 				                                                     //
// Purpose: just removes carrige return 					     	 //
///////////////////////////////////////////////////////////////////////
int remove_r(char *buff, int buff_len)
{
    char *ptr;

    ptr = buff;
    while (ptr = strstr(ptr, "\r\n"))
    {
        *ptr = '\n';
        ptr++;
        memmove(ptr, ptr + 1, buff_len - (int)(ptr - buff) - 1);
        buff_len--;
    }

    ptr = buff;
    while (ptr = strstr(ptr, "\n\r"))
    {
        *ptr = '\n';
        ptr++;
        memmove(ptr, ptr + 1, buff_len - (int)(ptr - buff) - 1);
        buff_len--;
    }

    return buff_len;
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

    char str_uip[MAX_BUFF];
    char str_cip[MAX_BUFF];
    char uip[4][MAX_BUFF];

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
    while (line = fgets(str_cip, MAX_BUFF, fp_checkIP))
    {
        if (str_cip[strlen(str_cip) - 1] == '\n')
            str_cip[strlen(str_cip) - 1] = '\0';
        i = 0;

        // convert check decimal ip to 32bits integer ////////////////////
        ptr = strtok(str_cip, ".");
        do
        {
            if (!strcmp(ptr, "*") || !strcmp(uip[i], ptr))
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
    char passwd[MAX_BUFF], buf[MAX_BUFF];
    int n, count = 1;

    while (1)
    {
        /* username를 client로부터 받는다 */
        read(connfd, buf, MAX_BUFF);
        strcpy(user, buf + 5);
        if ((n = userID_match(user)) == 1)
        { /* 인증 OK */
            strcpy(buf, "331 Password is required for username.\n");
            write(connfd, buf, MAX_BUFF);
            write(STDOUT_FILENO, buf, strlen(buf));
        }
        else if (n == 0)
        {
            write_log("LOG_FAIL");

            /////////// 3 times fail ///////////////
            if (count >= 3)
            {
                strcpy(buf, "530 Failed to log-in.\n");
                write(connfd, buf, MAX_BUFF);
                write(STDOUT_FILENO, buf, strlen(buf));
                return 0;
            }
            strcpy(buf, "430 Invalid username or password.\n");
            write(connfd, buf, MAX_BUFF);
            write(STDOUT_FILENO, buf, strlen(buf));
            count++;
            continue;
        }

        /* password를 client로부터 받는다 */
        read(connfd, buf, MAX_BUFF);
        strcpy(passwd, buf + 5);

        if ((n = user_match(user, passwd)) == 1)
        {
            /* 인증 OK */
            strcpy(buf, "230 User username logged in.\n");
            write(connfd, buf, MAX_BUFF);
            write(STDOUT_FILENO, buf, strlen(buf));
            break;
        }
        else if (n == 0)
        {
            write_log("LOG_FAIL");

            /////////// 3 times fail ///////////////
            if (count >= 3)
            {
                strcpy(buf, "530 Failed to log-in.\n");
                write(connfd, buf, MAX_BUFF);
                write(STDOUT_FILENO, buf, strlen(buf));
                return 0;
            }
            strcpy(buf, "430 Invalid username or password.\n");
            write(connfd, buf, MAX_BUFF);
            write(STDOUT_FILENO, buf, strlen(buf));
            count++;
            continue;
        }
    }

    write_log("LOG_IN");
    return 1;
}

///////////////////////////////////////////////////////////////////////
// userID_match                                                      //
// ================================================================= //
// Input: char * -> user ID									         //
// 		  char * -> user Password                                    //
// 				                                                     //
// Output: 1 -> success	                                             //
// 		   0 -> fail                                                 //
// 				                                                     //
// Purpose: Compare user ID with IDs in passwd			 			 //
// 				                                                     //
///////////////////////////////////////////////////////////////////////
int userID_match(char *user)
{
    FILE *fp;
    struct passwd *pw;

    fp = fopen("passwd", "r");

    //////////////// get passwd from file "passwd" /////////////////////
    while (pw = fgetpwent(fp))
    {
        //////////////// compare passwd information(user name) /////////////////////
        if (!strcmp(pw->pw_name, user))
            break;
    }

    fclose(fp);

    if (pw != NULL)
        return 1;
    else
        return 0;
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

///////////////////////////////////////////////////////////////////////
// write_log                                                         //
// ================================================================= //
// Input: const char *	-> comment		     						 //
// 				                                                     //
// Output: NON                    					     			 //
// 				                                                     //
// Purpose: write log to logfile according to format			     //
///////////////////////////////////////////////////////////////////////
void write_log(const char *str)
{
    char buf[MAX_BUFF];
    strcpy(buf, str);
    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    time(&t);
    timeinfo = localtime(&t);
    strftime(time_buff, sizeof(time_buff), "%a %b %d %H:%M:%S %Y", timeinfo);

    sprintf(log_buff, "%s [%s:%d] %s %s\n", time_buff, inet_ntoa(ctrl_client_addr.sin_addr), ctrl_client_addr.sin_port,
            user, buf);

    write(log_fd, log_buff, strlen(log_buff));
}

///////////////////////////////////////////////////////////////////////
// write_log_start_end                                               //
// ================================================================= //
// Input: const char *	-> comment		     						 //
// 				                                                     //
// Output: NON                    					     			 //
// 				                                                     //
// Purpose: write log to logfile according to format			     //
///////////////////////////////////////////////////////////////////////
void write_log_start_end(const char *str)
{
    char buf[MAX_BUFF];
    strcpy(buf, str);
    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    time(&t);
    timeinfo = localtime(&t);
    strftime(time_buff, sizeof(time_buff), "%a %b %d %H:%M:%S %Y", timeinfo);

    sprintf(log_buff, "%s %s\n", time_buff, buf);

    write(log_fd, log_buff, strlen(log_buff));
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

    write_log_start_end("Server is terminated");
    exit(1);
}
