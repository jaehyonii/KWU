///////////////////////////////////////////////////////////////////////
// File Name : kw20202020_ls.c                                       //
// Date : 2024/04/10                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim jae hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #1-2 ( ftp server )          //
// Description : Learn and practice                                  //
//               the functions related to directory		     //
//               ex) opendir(), readdir(), closedir()                //
///////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int ls(char* );

int main(int argc, char *argv[])
{
    char pathname[256]; // directory name which we want to open
    
    if(argc == 1) // if there's no arguments
	strcpy(pathname, "."); // we will open current working directory
    else if(argc == 2) // if there's one argument
	strcpy(pathname, argv[1]); // we will open it
    else
    {
	fprintf(stderr, "only one directory path can be processed\n"); // too many argument
	exit(1); // exit
    }

    if(ls(pathname) == -1) // if there's error
	// print error message
	fprintf(stderr, "kw2020202031_ls: cannot access '%s' : %s\n", pathname, strerror(errno));

    return 0;
}


///////////////////////////////////////////////////////////////////////
// ls                                                                //
// ================================================================= //
// Input: char* -> path name					     //
// 	  directoy name that we want to know 			     //
// 	  about the list of its internal files			     //
// 				                                     //
// Output: int -  0 success					     //
// 	       - -1 fail                                             //
// 	   return 0 if the function ls works well		     //
// 	   return -1 if the function ls occurs error                 //
// 				                                     //
// Purpose: open directory					     //
// 	    read all internal files and prints their name	     //
// 	    close directory				             //
///////////////////////////////////////////////////////////////////////

int ls(char* pathname){
    DIR *dp;
    struct dirent *dirp;
    
    if((dp = opendir(pathname)) == NULL) // open directory
	return -1;
    
    while(dirp = readdir(dp)) // read directory's internal files
        if(dirp->d_ino != 0)
	    printf("%s\n", dirp->d_name); // print the name of internal files
    
    if(closedir(dp) == -1) // close directory
        return -1;

    return 0; // success 0, error -1
}
