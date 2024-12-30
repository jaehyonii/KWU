#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PROCESSES 64

int main()
{
    FILE* rfile = fopen("./temp.txt", "r");
    FILE* wfile = fopen("./temp.txt", "a");
    
    int i, n;
    int value, value1, value2;
    char num[1024];
    struct timespec start, end;
    long sec, nsec;

    // clock start
    if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
    {
        perror("clock_gettime");
        return -1;
    }

    // read file and send two number to each process
    for(n = 0; n < MAX_PROCESSES; n++)
    {
        fgets(num, sizeof(num), rfile);
        value1 = atoi(num);
        fgets(num, sizeof(num), rfile);
        value2 = atoi(num);

        // child process add two number and exit
        if(fork() == 0)
        {
            value = value1 + value2;
            exit(value);
        }
    }

    // n is the number of nodes at each depth 
    for(n = MAX_PROCESSES / 2; n >= 1; n /= 2)
    {
        for(i = 0; i < n; i++)
        {
            // get child processes' exit status
            wait(&value1);
            wait(&value2);
            value1 >>= 8;
            value2 >>= 8;
            
            // write to temp.txt
            fprintf(wfile, "%d\n%d\n", value1, value2);
            fflush(wfile);
            if(fork() == 0)
            {
                value = value1 + value2;
                exit(value);
            }
        }
    }

    // get root process' exit status
    wait(&value);
    value >>= 8;
    fprintf(wfile, "%d\n", value);
    printf("value of fork: %d\n", value);

    // clock end
    if(clock_gettime(CLOCK_MONOTONIC, &end) == -1)
    {
        perror("clock_gettime");
        return -1;
    }

    // print clock time (format: xx.xxx)
    sec = end.tv_sec - start.tv_sec;
    nsec = end.tv_nsec - start.tv_nsec;
    if (nsec < 0) {
        sec -= 1;
        nsec += 1000000000;
    }
    printf("%ld.%09ld\n", sec, nsec);

    fclose(rfile);
    fclose(wfile);

    return 0;
}