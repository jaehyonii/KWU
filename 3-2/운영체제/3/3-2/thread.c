#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#define MAX_PROCESSES 64

// for sending two integer value to new thread
struct two_value{
    int tid;
    int value1;
    int value2;
};

// pthread_create function
void* sum_func(void* arg)
{
    int value, value1, value2;
    struct two_value * v = (struct two_value *)arg;
    
    value1 = v->value1;
    value2 = v->value2;

    value = value1 + value2;

    free(v);
    pthread_exit((void*)value);
}

int *array;

FILE* rfile;
FILE* wfile;

int main()
{    
    int i, n;
    char num[1024];
    
    struct timespec start, end;
    long sec, nsec;

    struct two_value *v;

    pthread_t tid[MAX_PROCESSES];

    // the number of number in temp.txt is MAX_PROCESSES * 2
    array = (int*)malloc(sizeof(int) * MAX_PROCESSES * 2);

    rfile = fopen("./temp.txt", "r");
    wfile = fopen("./temp.txt", "a");

    // clock start
    if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
    {
        perror("clock_gettime");
        return -1;
    }

    // save numbers in temp.txt into array
    for(n = 0; n < MAX_PROCESSES * 2; n++)
    {
        fgets(num, sizeof(num), rfile);
        array[n] = atoi(num);
    }

    // n is the number of nodes at each depth 
    for(n = MAX_PROCESSES; n >= 1; n /= 2)
    {
        // create threads
        for(i = 0; i < n; i++)
        {
            v = (struct two_value*)malloc(sizeof(struct two_value));
            v->tid = i;
            v-> value1 = array[2 * i];
            v-> value2 = array[2 * i + 1];
            pthread_create(&tid[i], NULL, sum_func, (void*)v);
        }

        // join threads
        for(i = 0; i < n; i++)
        {
            pthread_join(tid[i], (void**)&array[i]);
            // write to temp.txt
            fprintf(wfile, "%d\n", array[i]);
        }
    }
    
    printf("value of thread: %d\n", array[0]);

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

    free(array);
    
    return 0;
}