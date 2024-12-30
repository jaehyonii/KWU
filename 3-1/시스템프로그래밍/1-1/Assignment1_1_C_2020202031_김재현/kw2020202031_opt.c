///////////////////////////////////////////////////////////////////////
// File Name : kw20202020_opt.c                                      //
// Date : 2024/03/30                                                 //
// OS : Ubuntu 20.04.6 LTS 64bits                                    //
// Author : Kim jae hyun                                             //
// Student ID : 2020202031                                           //
// ----------------------------------------------------------------- //
// Title: System Programming Assignment #1-1 ( ftp server )          //
// Description : Learn and practice                                  //
//               the getopt function in unistd header                //
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// main                                                              //
// ================================================================= //
// Input: int -> The number of parameter,                            //
//        char ** -> Vector of Strings                               //
// (Input parameter Description)                                     //
// Output: int 0                                                     //
// (Out parameter Description)                                       //
// Purpose: figuring out option information of Input                 //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>

 int main (int argc, char **argv)
 {
    int aflag = 0, bflag = 0; // the number of option a, b
    char *cvalue = NULL;
    int c; // each option
    opterr = 0; // disable error message printing
    
    while ((c = getopt (argc, argv, "abc:")) != -1)
    {
        switch (c)
        {
            case 'a': // if option is -a
                aflag++;
                break;
            
            case 'b': // if option is -b
                bflag++;
                break;

            case 'c': // if option is -c
                cvalue = optarg; // option of option c
                break;
        }
    }

    //=============print flags and c's argument=============//
    printf("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);
    
    //=============print non-option arguments=============//
    for(int i = optind; i<argc; i++)
        printf("Non-option argument %s\n", argv[i]);

    printf("\n");

    return 0;
 }