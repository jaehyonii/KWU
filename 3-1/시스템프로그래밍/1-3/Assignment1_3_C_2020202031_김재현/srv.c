///////////////////////////////////////////////////////////////////////
// File Name : srv.c                                                 //
// Date : 2024/04/18                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim jae hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #1-3 ( ftp server )          //
// Description : Learn and practice about FTP commands               //
//         Make program that converts client command to FTP commands //
//         Make program that runs FTP commands                       //
///////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#define MAX_BUFF 4096

int argc = 0;
char *argv[100];
int aflag = 0, lflag = 0, non_flag = 0;

int NLST(const char *pathname);
int LIST(const char *pathname);
int PWD();
int CWD(const char *pathname);
int CDUP();
int MKD(const char *pathname);
int DELE(const char *pathname);
int RMD(const char *pathname);
int RN(const char *pathname, const char *pathname2);

int func_compare(const void *first, const void *second);
const char *get_filename(const char *path);
int print_l(const char *name);

///////////////////////////////////////////////////////////////////////
// main                                                              //
// ================================================================= //
// Input: NONE   			                                         //
// 				                                                     //
// Output: NONE                                                      //
// 				                                                     //
// Purpose: Run FTP commands                        			     //
///////////////////////////////////////////////////////////////////////
int main()
{
    char buf[MAX_BUFF]; // buffer
    char tmp_buf[MAX_BUFF];
    char *ptr; // string pointer
    int rd;    // read byte
    int c = 0; // get option
    struct stat st;
    int i, j;

    rd = read(0, buf, MAX_BUFF);

    // token command by delimiter ' '
    // save strings each in cmd
    argv[0] = strtok(buf, " ");
    argc++;
    while (ptr = strtok(NULL, " "))
    {
        argv[argc] = ptr;
        argc++;
    }

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
    // invalid option exception
    if (non_flag > 0 || (strncmp(argv[0], "NLST", 5) && aflag + lflag + non_flag > 0))
    {
        ptr = "Error: invalid option\n\n";
        write(2, ptr, strlen(ptr));
        exit(1);
    }
    // no command exception
    if (!strncmp(argv[0], "NON", 4))
    {
        ptr = "Error: no command exists\n\n";
        write(2, ptr, strlen(ptr));
        exit(1);
    }
    // wrong command exception
    if (!strncmp(argv[0], "WRONG", 6))
    {
        ptr = "Error: wrong command\n\n";
        write(2, ptr, strlen(ptr));
        exit(1);
    }

    ////////////// execute command ///////////////////
    // NLST
    if (!strncmp(argv[0], "NLST", 5))
    {
        // print command
        j = 0;
        for (i = 0; i < optind; i++)
            j += sprintf(tmp_buf + j, "%s ", argv[i]);
        write(1, tmp_buf, strlen(tmp_buf));
        write(1, "\n", 1);

        // Error handling
        if (argc - optind > 1)
        {
            sprintf(tmp_buf, "Error: too many arguments...\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (NLST(argv[optind]) == -1)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
    }

    // LIST
    else if (!strncmp(argv[0], "LIST", 5))
    {
        // print command
        write(1, "LIST\n", 5);

        // Error handling
        if (argc - optind > 1)
        {
            sprintf(tmp_buf, "Error: too many arguments\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (LIST(argv[optind]) == -1)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
    }

    // PWD
    else if (!strncmp(argv[0], "PWD", 4))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(tmp_buf, "Error: argument is not required\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (PWD() < -1)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
    }

    // CWD
    else if (!strncmp(argv[0], "CWD", 4))
    {
        // Error handling
        if (argc - optind > 1)
        {
            sprintf(tmp_buf, "Error: too many arguments...\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (CWD(argv[optind]) == -1)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
    }

    // CDUP
    else if (!strncmp(argv[0], "CDUP", 5))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(tmp_buf, "Error: too many arguments...\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (CDUP(argv[optind]) == -1)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
    }

    // MKD
    else if (!strncmp(argv[0], "MKD", 4))
    {
        // Error handling
        if (argc - optind == 0)
        {
            sprintf(tmp_buf, "Error: argument is required\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }

        for (int i = optind; argc - i > 0; i++)
        {
            // Error handling
            if (MKD(argv[i]) < 0)
            {
                if (errno == EEXIST)
                {
                    sprintf(tmp_buf, "Error: cannot create directory '%s': file exists\n", argv[i]);
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
                else
                {
                    sprintf(tmp_buf, "Error: %s\n", strerror(errno));
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
            }

            // print FTP command
            sprintf(tmp_buf, "MKD %s\n", argv[i]);
            write(1, tmp_buf, strlen(tmp_buf));
        }
        write(1, "\n", 1);
    }

    // DELE
    else if (!strncmp(argv[0], "DELE", 5))
    {
        // Error handling
        if (argc - optind == 0)
        {
            sprintf(tmp_buf, "Error: argument is required\n\n");
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }

        for (int i = optind; argc - i > 0; i++)
        {
            // Error handling
            if (DELE(argv[i]) < 0)
            {
                if (errno == ENOENT)
                {
                    sprintf(tmp_buf, "Error: failed to delete '%s' \n", argv[i]);
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
                else
                {
                    sprintf(tmp_buf, "Error: %s\n", strerror(errno));
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
            }

            // print FTP command
            sprintf(tmp_buf, "DELE %s\n", argv[i]);
            write(1, tmp_buf, strlen(tmp_buf));
        }
        write(1, "\n", 1);
    }

    // RMD
    else if (!strncmp(argv[0], "RMD", 4))
    {
        if (argc - optind == 0)
        {
            sprintf(tmp_buf, "Error: argument is required\n\n");
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }

        for (int i = optind; argc - i > 0; i++)
        {
            // Error handling
            if (RMD(argv[i]) < 0)
            {
                if (errno == ENOENT)
                {
                    sprintf(tmp_buf, "Error: failed to remove '%s'\n", argv[i]);
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
                else
                {
                    sprintf(tmp_buf, "Error: %s\n", strerror(errno));
                    write(2, tmp_buf, strlen(tmp_buf));
                    continue;
                }
            }

            // print FTP command
            sprintf(tmp_buf, "RMD %s\n", argv[i]);
            write(1, tmp_buf, strlen(tmp_buf));
        }
        write(1, "\n", 1);
    }

    // RNFR & RNTO
    else if (!strncmp(argv[0], "RNFR", 5))
    {
        // Error handling
        if (argc - optind != 3)
        {
            sprintf(tmp_buf, "Error: two arguments are required\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (stat(argv[3], &st) == 0 && !S_ISDIR(st.st_mode))
        {
            sprintf(tmp_buf, "Error: name to change already exists\n\n");
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }
        if (RN(argv[1], argv[3]) < 0)
        {
            sprintf(tmp_buf, "Error: %s\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
        }

        // print command
        sprintf(tmp_buf, "RNFR %s\nRNTO %s\n\n", argv[1], argv[3]);
        write(2, tmp_buf, strlen(tmp_buf));        
    }

    // QUIT
    else if (!strncmp(argv[0], "QUIT", 5))
    {
        // Error handling
        if (argc - optind > 0)
        {
            sprintf(tmp_buf, "Error: argument is not required\n\n", strerror(errno));
            write(2, tmp_buf, strlen(tmp_buf));
            exit(1);
        }

        // print command
        sprintf(tmp_buf, "QUIT success\n\n");
        write(2, tmp_buf, strlen(tmp_buf));
    }

    exit(0); // srv exit
}

///////////////////////////////////////////////////////////////////////
// get_filename                                                      //
// ================================================================= //
// Input: const char * -> path 					                     //
// 				                                                     //
// Output: const char * -> file name                                 //
// 				                                                     //
// Purpose: Extract filename from path                               //
///////////////////////////////////////////////////////////////////////
const char *get_filename(const char *path)
{
    const char *filename = strrchr(path, '/');
    if (filename == NULL) { // 경로에 '/' 문자가 없는 경우
        return path;
    } else { // 경로에 '/' 문자가 있는 경우
        return filename + 1;
    }
}

///////////////////////////////////////////////////////////////////////
// print_l                                                           //
// ================================================================= //
// Input: const char * -> path 					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Print the information of file named path                 //
// Information : type, permission, no.link,	USER name, GROUP name,   //
// 			     size of file, last modification time, filename 	 //
///////////////////////////////////////////////////////////////////////
int print_l(const char *name)
{
    char buff[MAX_BUFF];
    char *filename;
    struct stat st;               // stat of dirp
    struct passwd *userInfo;      // User information
    struct group *groupInfo;      // Group information
    struct tm *modification_time; //
    int j;

    if (stat(name, &st) < 0)
        return -1;
    if ((userInfo = getpwuid(st.st_uid)) == NULL)
        return -1;
    if ((groupInfo = getgrgid(st.st_gid)) == NULL)
        return -1;
    modification_time = localtime(&st.st_mtime);
    j = sprintf(buff, "%c", ((S_ISDIR(st.st_mode)) ? 'd' : '-'));
    j += sprintf(buff + j, "%c%c%c", (st.st_mode & S_IRUSR) ? 'r' : '-', (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-');
    j += sprintf(buff + j, "%c%c%c", (st.st_mode & S_IRGRP) ? 'r' : '-', (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-');
    j += sprintf(buff + j, "%c%c%c ", (st.st_mode & S_IROTH) ? 'r' : '-', (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
    j += sprintf(buff + j, "%3d\t%s\t%s\t%5d\t", st.st_nlink, userInfo->pw_name, groupInfo->gr_name, st.st_size);
    j += strftime(buff + j, sizeof(buff) - j, "%b %d %H:%M ", modification_time);
    filename = get_filename(name);
    j += sprintf(buff + j, "%s", filename);
    write(1, buff, strlen(buff));
    if (S_ISDIR(st.st_mode))
        write(1, "/", 1);
    write(1, "\n", 1);

    return 0;
}

///////////////////////////////////////////////////////////////////////
// NLST                                                              //
// ================================================================= //
// Input: const char * -> path 					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Print the information of subfiles of directory named path//
// 	     or Print the information of files named path                //
///////////////////////////////////////////////////////////////////////
int NLST(const char *pathname)
{
    DIR *dp;
    struct dirent *dirp;
    struct dirent *dirpp[MAX_BUFF];
    int num = 0;         // number of dirp
    char buff[MAX_BUFF]; // write buffer
    char name_buff[MAX_BUFF];
    struct stat st;               // stat of dirp
    char *name = NULL;            // file name
    struct tm *modification_time; //
    int i, j, k;

    memset(buff, 0, MAX_BUFF);

    // if there's no pathname argument, pathname is "."
    if (pathname == NULL)
        pathname = ".";

    if (stat(pathname, &st) < 0)
        return -1;

    // if pathname is filename
    if (S_ISREG(st.st_mode))
    {
        // no option
        if (lflag == 0)
        {
            write(1, pathname, strlen(pathname));
            write(1, "\n\n", 2);
        }
        // l option
        else
        {
            if (print_l(pathname) < 0)
            {
                write(1, "\n", 1);
                return -1;
            }
            write(1, "\n", 1);
        }

        return 0;
    }

    // control directory only
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
    if (lflag > 0)
    {
        for (i = 0; i < num; i++)
        {
            name = dirpp[i]->d_name;
            strcpy(name_buff, pathname);
            strcat(name_buff, "/");
            strcat(name_buff, name);
            if (aflag == 0 && name[0] == '.')
                continue;

            if (print_l(name_buff) < 0)
                continue;
            memset(buff, 0, MAX_BUFF);
        }
        write(1, "\n", 1);
    }
    else
    {
        k = 0;
        j = 0;
        for (i = 0; i < num; i++)
        {
            name = dirpp[i]->d_name;
            strcpy(name_buff, pathname);
            strcat(name_buff, "/");
            strcat(name_buff, name);

            // aflag OFF
            if (aflag == 0 && name[0] == '.')
                continue;

            if (stat(name_buff, &st) < 0)
                continue;

            j += sprintf(buff + j, "%s", name);
            if (S_ISDIR(st.st_mode))
                j += sprintf(buff + j, "/");
            j += (++k % 5 != 0) ? sprintf(buff + j, "\t") : sprintf(buff + j, "\n");
        }
        write(1, buff, strlen(buff));
        write(1, "\n\n", 2);
    }

    // close directory
    if (closedir(dp) < 0)
        return -1;

    return 0; // success 0, error -1
}

///////////////////////////////////////////////////////////////////////
// LIST                                                              //
// ================================================================= //
// Input: const char * -> path 					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Operate the same as NLST -al                             //
///////////////////////////////////////////////////////////////////////
int LIST(const char *pathname)
{
    DIR *dp;
    struct dirent *dirp;
    struct dirent *dirpp[MAX_BUFF];
    int num = 0;         // number of dirp
    char buff[MAX_BUFF]; // write buffer
    char name_buff[MAX_BUFF];
    struct stat st;               // stat of dirp
    char *name = NULL;            // file name
    struct tm *modification_time; //
    int i, j, k;

    memset(buff, 0, MAX_BUFF);

    // if there's no pathname argument, pathname is "."
    if (pathname == NULL)
        pathname = ".";

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
    if (1)
    {
        for (i = 0; i < num; i++)
        {
            name = dirpp[i]->d_name;
            strcpy(name_buff, pathname);
            strcat(name_buff, "/");
            strcat(name_buff, name);
            if (print_l(name) < 0)
                continue;
            memset(buff, 0, MAX_BUFF);
        }
    }

    // close directory
    if (closedir(dp) < 0)
        return -1;

    return 0; // success 0, error -1
}

///////////////////////////////////////////////////////////////////////
// PWD                                                               //
// ================================================================= //
// Input: NONE               					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Print path of current working directory 		         //
///////////////////////////////////////////////////////////////////////
int PWD()
{
    char buf[MAX_BUFF];
    char wd[MAX_BUFF];

    if (getcwd(wd, MAX_BUFF) == NULL)
        return -1;

    sprintf(buf, "\"%s\" is current directory\n\n", wd);
    write(1, buf, strlen(buf));

    return 0;
}

///////////////////////////////////////////////////////////////////////
// CWD                                                               //
// ================================================================= //
// Input: const char * -> path 					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Change location to path                			         //
// 			Print path of current working directory                  //
///////////////////////////////////////////////////////////////////////
int CWD(const char *pathname)
{
    char buf[MAX_BUFF];
    char wd[MAX_BUFF];

    if (chdir(pathname) < 0)
        return -1;

    if (getcwd(wd, MAX_BUFF) == NULL)
        return -1;

    // print command
    sprintf(buf, "CWD %s\n", pathname);
    write(1, buf, strlen(buf));

    sprintf(buf, "\"%s\" is current directory\n\n", wd);
    write(1, buf, strlen(buf));

    return 0;
}

///////////////////////////////////////////////////////////////////////
// CDUP                                                              //
// ================================================================= //
// Input: NONE              					                     //
// 				                                                     //
// Output: int -  0 success                                          //
// 				 -1 error                                            //
// 				                                                     //
// Purpose: Change location to Parent directory   			         //
// 			Print path of current working directory                  //
///////////////////////////////////////////////////////////////////////
int CDUP()
{
    char buf[MAX_BUFF];
    char wd[MAX_BUFF];

    if (chdir("..") < 0)
        return -1;

    if (getcwd(wd, MAX_BUFF) == NULL)
        return -1;

    // print command
    sprintf(buf, "CDUP\n");
    write(1, buf, strlen(buf));

    sprintf(buf, "\"%s\" is current directory\n\n", wd);
    write(1, buf, strlen(buf));

    return 0;
}

///////////////////////////////////////////////////////////////////////
// MKD                                                               //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int - return value of function mkdir()                    //
// 				                                                     //
// Purpose: just for using function mkdir()    			    		 //
///////////////////////////////////////////////////////////////////////
int MKD(const char *pathname)
{
    return mkdir(pathname, 0775);
}

///////////////////////////////////////////////////////////////////////
// DELE                                                              //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int - return value of function unlink()                   //
// 				                                                     //
// Purpose: just for using function unlink()    					 //
///////////////////////////////////////////////////////////////////////
int DELE(const char *pathname)
{
    return unlink(pathname);
}

///////////////////////////////////////////////////////////////////////
// RMD                                                               //
// ================================================================= //
// Input: const void * -> path					                     //
// 				                                                     //
// Output: int - return value of function rmdir()                    //
// 				                                                     //
// Purpose: just for using function rmdir()    					     //
///////////////////////////////////////////////////////////////////////
int RMD(const char *pathname)
{
    return rmdir(pathname);
}

///////////////////////////////////////////////////////////////////////
// RN                                                                //
// ================================================================= //
// Input: const void * -> path1					                     //
// 	      const void * -> path2                                      //
// 				                                                     //
// Output: int - return value of function rename()                   //
// 				                                                     //
// Purpose: just for using function rename() 					     //
///////////////////////////////////////////////////////////////////////
int RN(const char *pathname1, const char *pathname2)
{
    return rename(pathname1, pathname2);
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