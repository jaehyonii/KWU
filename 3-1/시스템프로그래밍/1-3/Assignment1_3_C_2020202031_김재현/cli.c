///////////////////////////////////////////////////////////////////////
// File Name : cli.c                                                 //
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF 4096

///////////////////////////////////////////////////////////////////////
// main                                                              //
// ================================================================= //
// Input: int argc -> the number of arguments					     //
// 	      char *argv[] -> parsing Inputs by space    			     //
// 				                                                     //
// Output: NONE                                                      //
// 				                                                     //
// Purpose: Converts USER command to FTP command    			     //
///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    char buf[MAX_BUFF];
    char *ptr;

    memset(buf, 0, MAX_BUFF);

    // no USER command
    if(argc == 1)
    {
        strncpy(buf, "NON", 6);
    }
    // ls -> NLST
    else if(!strncmp((argv[1]), "ls", -1))
    {
        strncpy(buf, "NLST", 5);
    }
    // dir -> LIST
    else if(!strncmp((argv[1]), "dir", -1))
    {
        strncpy(buf, "LIST", 5);
    }
    // pwd -> PWD
    else if(!strncmp((argv[1]), "pwd", -1))
    {
        strncpy(buf, "PWD", 4);
    }
    // cd -> CWD or CDUP
    else if(!strncmp((argv[1]), "cd", -1))
    {
        // cd -> CWD
        strncpy(buf, "CWD", 3);
        
        for(int i = 2; i < argc; i++)
        {
            // cd -> CDUP
            if(!strncmp(argv[i], "..", 3))
            {
                strncpy(buf, "CDUP", 5);
                // remove argument ".."
                for(int j = i; j< argc - 1; j++)
                {
                    argv[j] = argv[j + 1];
                }
                argc--;
                break;
            }
        }
    }
    // mkdir -> MKD
    else if(!strncmp((argv[1]), "mkdir", -1))
    {
        strncpy(buf, "MKD", 4);
    }
    // delete -> DELE
    else if(!strncmp((argv[1]), "delete", -1))
    {
        strncpy(buf, "DELE", 5);
    }
    // rmdir -> RMD
    else if(!strncmp((argv[1]), "rmdir", -1))
    {
        strncpy(buf, "RMD", 4);
    }
    // rename -> RNFR & RNTO
    else if(!strncmp((argv[1]), "rename", -1))
    {
        sprintf(buf, "RNFR %s RNTO", argv[2]);
    }
    // quit -> QUIT
    else if(!strncmp((argv[1]), "quit", -1))
    {
        strncpy(buf, "QUIT", 5);
    }
    // USER command is incorrect
    else
    {
        strncpy(buf, "WRONG", 5);
    }

    // write arguments to the buffer
    for(int i = 2; i < argc; i++)
    {
        if(!strncmp(argv[1], "rename", -1) && i < 3)
            continue;
        buf[strlen(buf)] = ' ';
        strcat(buf, argv[i]);
    }

    // write buffer
    write(1, buf, MAX_BUFF);

    // exit
    exit(0);
}