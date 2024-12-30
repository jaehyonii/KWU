#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Q_SIZE 1000  // size of QUEUE

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
}Process;

typedef struct {
    int items[Q_SIZE];
    int front, rear;
} Queue;

void initializeQueue(Queue *q) {
    q->front = q->rear = -1;
}

int isEmpty(Queue *q) {
    return q->front == -1;
}

int isFull(Queue *q) {
    return (q->rear + 1) % Q_SIZE == q->front;
}

void enqueue(Queue *q, int value) {
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    if (isEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % Q_SIZE;
    }
    q->items[q->rear] = value;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % Q_SIZE;
    }
    return item;
}

int compareArrivalTime(const void *a, const void *b) {
    return (((Process *)a)->arrival_time - ((Process *)b)->arrival_time);
}

Process processes[1000]; // process ascending order by arrival_time
int pnum; // the number of processes

void FCFS()
{
    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    int totalBurstTime = 0;
    double totalCpuTime = 0; // final currentTime + 0.1 * the number of context switching ( = the number of processes)

    printf("Gantt Chart:\n");
    for (int i = 0; i < pnum; i++) {
        int start_time = (currentTime > processes[i].arrival_time) ? currentTime : processes[i].arrival_time;
        int completion_time = start_time + processes[i].burst_time;
        int waiting_time = start_time - processes[i].arrival_time;
        int turnaround_time = completion_time - processes[i].arrival_time;

        totalWaitingTime += waiting_time;
        totalTurnaroundTime += turnaround_time;
        totalBurstTime += processes[i].burst_time;

        currentTime = completion_time;
        for(int j = 0; j < processes[i].burst_time; j++)
            printf("| P%d ", processes[i].pid);
    }
    printf("|\n");

    totalCpuTime = (double)currentTime + 0.1 * pnum;

    printf("Average Waiting Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("Average Turnaround Time = %.2f\n", (double)totalTurnaroundTime / pnum);
    printf("Average Response Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("CPU Utilization = %.2f%%\n", totalBurstTime / totalCpuTime * 100);
}

void RR(int quantum)
{
    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;
    int totalBurstTime = 0;
    double totalCpuTime = 0; // final currentTime + 0.1 * the number of context switching ( = the number of processes)
    int contextSwitch = 0;

    int completed = 0, index = 0;
    int inQueue[Q_SIZE];
    Queue q;

    initializeQueue(&q);
    
    for(int i = 0; i < Q_SIZE; i++)
        inQueue[i] = 0;
    
    printf("Gantt Chart:\n");
    while (completed < pnum) {
        // insert arrived processes into queue
        for (int i = 0; i < pnum; i++) {
            if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0 && inQueue[i] == 0) {
                enqueue(&q, i);
                inQueue[i] = 1; // make mark that it is in queue
            }
        }

        if (!isEmpty(&q)) {
            index = dequeue(&q);
            contextSwitch++;
            if(processes[index].response_time == -1)
                processes[index].response_time = currentTime - processes[index].arrival_time;
            if (processes[index].remaining_time > quantum) {
                // process isn't completed during quantum time
                for(int j = 0; j < quantum; j++)
                    printf("| P%d ", processes[index].pid);
                currentTime += quantum;
                processes[index].remaining_time -= quantum;
            } else {
                // process is completed during quantum time
                for(int j = 0; j < processes[index].remaining_time; j++)
                    printf("| P%d ", processes[index].pid);
                currentTime += processes[index].remaining_time;
                processes[index].waiting_time = currentTime - processes[index].arrival_time - processes[index].burst_time;
                processes[index].turnaround_time = currentTime - processes[index].arrival_time;
                processes[index].remaining_time = 0;
                completed++;
            }

            // insert processes arrived during quantum into queue
            for (int i = 0; i < pnum; i++) {
                if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0 && inQueue[i] == 0) {
                    enqueue(&q, i);
                    inQueue[i] = 1; // make mark that it is in queue
                }
            }
            inQueue[index] = 0;
        } else {
            // there is no process in queue
            currentTime++;
        }
    }
    printf("|\n");
    
    totalCpuTime = (double)currentTime + 0.1 * contextSwitch;

    for(int i = 0; i < pnum; i++)
    {
        totalWaitingTime += processes[i].waiting_time;
        totalTurnaroundTime += processes[i].turnaround_time;
        totalResponseTime += processes[i].response_time;
        totalBurstTime += processes[i].burst_time;
    }
    printf("Average Waiting Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("Average Turnaround Time = %.2f\n", (double)totalTurnaroundTime / pnum);
    printf("Average Response Time = %.2f\n", (double)totalResponseTime / pnum);
    printf("CPU Utilization = %.2f%%\n", totalBurstTime / totalCpuTime * 100);
}

void SJF()
{
    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    int totalBurstTime = 0;
    double totalCpuTime = 0; // final currentTime + 0.1 * the number of context switching ( = the number of processes)
    
    int isCompleted[pnum];
    int completed = 0; // the number of completed processes
    
    for (int i = 0; i < pnum; i++) {
        isCompleted[i] = 0;
    }

    printf("Gantt Chart:\n");
    while (completed < pnum) {
        int shortestJobIndex = -1;
        int minimumBurstTime = 1e9;

        // find process which already arrives and has shortest job
        for (int i = 0; i < pnum; i++) {
            if (processes[i].arrival_time <= currentTime && !isCompleted[i]) {
                if (processes[i].burst_time < minimumBurstTime) {
                    minimumBurstTime = processes[i].burst_time;
                    shortestJobIndex = i;
                }
            }
        }

        if (shortestJobIndex == -1) {
            currentTime++;
        } else {
            int start_time = currentTime;
            int completion_time = start_time + processes[shortestJobIndex].burst_time;
            int waiting_time = start_time - processes[shortestJobIndex].arrival_time;
            int turnaround_time = completion_time - processes[shortestJobIndex].arrival_time;

            totalWaitingTime += waiting_time;
            totalTurnaroundTime += turnaround_time;
            totalBurstTime += processes[shortestJobIndex].burst_time;

            currentTime = completion_time;
            isCompleted[shortestJobIndex] = 1;
            completed++;

            for(int j = 0; j < processes[shortestJobIndex].burst_time; j++)
                printf("| P%d ", processes[shortestJobIndex].pid);
        }
    }
    printf("|\n");

    totalCpuTime = (double)currentTime + 0.1 * pnum;

    printf("Average Waiting Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("Average Turnaround Time = %.2f\n", (double)totalTurnaroundTime / pnum);
    printf("Average Response Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("CPU Utilization = %.2f%%\n", totalBurstTime / totalCpuTime * 100);
}

void SRTF()
{
    int completed = 0, currentTime = 0;
    int min_remaining_time, index, prev_index = -1; // SRT process and its index
    int totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;
    int totalBurstTime = 0;
    int contextSwitch = 0;
    int isProcessAvailable;
    double totalCpuTime = 0; // final currentTime + 0.1 * the number of context switching ( = the number of processes)

    printf("Gantt Chart:\n");
    while (completed < pnum) {
        min_remaining_time = __INT_MAX__;
        index = -1;
        isProcessAvailable = 0;

        // find SRT process
        for (int i = 0; i < pnum; i++) {
            if (processes[i].arrival_time <= currentTime && processes[i].remaining_time > 0) {
                isProcessAvailable = 1;
                if (processes[i].remaining_time < min_remaining_time) {
                    min_remaining_time = processes[i].remaining_time;
                    index = i;
                }
            }
        }

        if (!isProcessAvailable) {
            // there isn't excutable process
            currentTime++;
            continue;
        }

        // process excutes
        if(index != prev_index)
        {    
            prev_index = index;
            contextSwitch++;
        }
        if(processes[index].response_time == -1)
            processes[index].response_time = currentTime - processes[index].arrival_time;
        processes[index].remaining_time--;
        currentTime++;
        printf("| P%d ", processes[index].pid);

        // process completes
        if (processes[index].remaining_time == 0) {
            completed++;
            processes[index].turnaround_time = currentTime - processes[index].arrival_time;
            processes[index].waiting_time = processes[index].turnaround_time - processes[index].burst_time;
        }
    }
    printf("|\n");
    totalCpuTime = (double)currentTime + 0.1 * contextSwitch;

    for(int i = 0; i < pnum; i++)
    {
        totalWaitingTime += processes[i].waiting_time;
        totalTurnaroundTime += processes[i].turnaround_time;
        totalResponseTime += processes[i].response_time;
        totalBurstTime += processes[i].burst_time;
    }

    printf("Average Waiting Time = %.2f\n", (double)totalWaitingTime / pnum);
    printf("Average Turnaround Time = %.2f\n", (double)totalTurnaroundTime / pnum);
    printf("Average Response Time = %.2f\n", (double)totalResponseTime / pnum);
    printf("CPU Utilization = %.2f%%\n", totalBurstTime / totalCpuTime * 100);
}

int main(int argc, char** argv)
{
    FILE *rfile;
    char line[100];

    
    if(argc >4)
    {    
        printf("Too many arguments\n");
        return 1;
    }

    if ((rfile = fopen(argv[1], "r")) == NULL) {
        printf("file opening fails\n");
        return 1;
    }

    while(fgets(line, sizeof(line), rfile))
    {
        char *token;
        int num[3];
        int i = 0;

        token = strtok(line, " ");
        while (token != NULL) {
            num[i++] = atoi(token);
            token = strtok(NULL, " ");
        }
        processes[pnum].pid = num[0];
        processes[pnum].arrival_time = num[1];
        processes[pnum].burst_time = num[2];
        processes[pnum].remaining_time = num[2];
        processes[pnum].waiting_time = -1;
        processes[pnum].turnaround_time = -1;
        processes[pnum].response_time = -1;

        pnum++;
    }
    qsort(processes, pnum, sizeof(Process), compareArrivalTime);

    if(strcmp(argv[2], "FCFS") == 0)
    {
        FCFS();
    }
    else if(strcmp(argv[2], "RR") == 0)
    {
        RR(atoi(argv[3]));
    }
    else if(strcmp(argv[2], "SJF") == 0)
    {
        SJF();
    }

    else if(strcmp(argv[2], "SRTF") == 0)
    {
        SRTF();
    }
    
    fclose(rfile);

    return 0;
}