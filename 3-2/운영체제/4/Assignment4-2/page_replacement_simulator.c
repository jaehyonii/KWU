#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FRAME 1000

int findOptimalFrameToReplace(int *pages, int *frames, int frameCount, int pageCount, int currentIndex) {
    int farthest = currentIndex;
    int frameToReplace = -1;

    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = currentIndex; j < pageCount; j++) {
            if (frames[i] == pages[j]) {
                // if the page appears later comparing with previous pages
                if (j > farthest) {
                    farthest = j;
                    frameToReplace = i;
                }
                break;
            }
        }
        // it never appears in pages
        if (j == pageCount) {
            frameToReplace = i;
            break;
        }
    }

    // all future pages don't exist in frames
    if (frameToReplace == -1)
        return 0; // just return 0
    else
        return frameToReplace;
}

void OPT(int* pages, int pageCount, int frameCount)
{
    int frames[MAX_FRAME];
    int frameSize = 0;
    int numPageFaults = 0;

    // frames is empty
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    for(int i = 0; i < pageCount; i++)
    {
        // check if page exists in frames
        int valid = 0;
        for(int j = 0; j < frameCount; j++)
        {
            if(pages[i] == frames[j])
            {
                valid = 1;
                break;
            }
        }

        // page already exists in frames
        if(valid)
            continue;
        
        // there are empty frame
        if(frameSize < frameCount)
            frames[frameSize++] = pages[i];
        else
        {
            int frameToReplace = findOptimalFrameToReplace(pages, frames, frameCount, pageCount, i + 1);
            frames[frameToReplace] = pages[i];
        }
        numPageFaults++;
    }

    printf("Optimal Algorithm:\n");
    printf("Number Of Page Faults : %d\n", numPageFaults);
    printf("Page Fault Rate: %.2lf%%\n\n", (double)numPageFaults / pageCount * 100);
}

void FIFO(int* pages, int pageCount, int frameCount)
{
    int frames[MAX_FRAME];
    int frameStart = 0, frameEnd = 0;
    int frameSize = 0;
    int numPageFaults = 0;

    // frames is empty
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    for(int i = 0; i < pageCount; i++)
    {
        // check if page exists in frames
        int valid = 0;
        for(int j = 0; j < frameCount; j++)
        {
            if(pages[i] == frames[j])
            {
                valid = 1;
                break;
            }
        }

        // page already exists in frames
        if(valid)
            continue;
        
        // there are empty frame
        if(frameSize < frameCount)
        {    
            frames[frameEnd] = pages[i];
            frameEnd = (frameEnd + 1) % frameCount;
            frameSize++;
        }
        else
        {
            frames[frameEnd] = pages[i];
            frameStart = (frameStart + 1) %frameCount;
            frameEnd = (frameEnd + 1) % frameCount;
        }
        numPageFaults++;        
    }


    printf("FIFO Algorithm:\n");
    printf("Number Of Page Faults : %d\n", numPageFaults);
    printf("Page Fault Rate: %.2lf%%\n\n", (double)numPageFaults / pageCount * 100);
}

void LRU(int* pages, int pageCount, int frameCount)
{
    int frames[MAX_FRAME];
    int frameStart = 0, frameEnd = 0;
    int frameSize = 0;
    int numPageFaults = 0;

    // frames is empty
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    for(int i = 0; i < pageCount; i++)
    {
        // check if page exists in frames
        int valid = 0;
        int j;
        for(j = 0; j < frameCount; j++)
        {
            if(pages[i] == frames[j])
            {
                valid = 1;
                break;
            }
        }

        // page already exists in frames
        if(valid)
        {
            // move frame that corresponds to this page to top
            int tmp = frames[j];
            int k, l;
            
            k = j;
            l = (j + 1) % frameCount;
            while(l != frameEnd)
            {
                frames[k] = frames[l];
                k = l;
                l = (l + 1) % frameCount;
            }
            if(frameEnd == 0)
                frames[frameCount - 1] = tmp;
            else
                frames[frameEnd - 1] = tmp;
            continue;
        }
        
        // there are empty frames
        if(frameSize < frameCount)
        {    
            frames[frameEnd] = pages[i];
            frameEnd = (frameEnd + 1) % frameCount;
            frameSize++;
        }
        else
        {
            frames[frameEnd] = pages[i];
            frameStart = (frameStart + 1) % frameCount;
            frameEnd = (frameEnd + 1) % frameCount;
        }
        numPageFaults++;        
    }
    
    printf("LRU Algorithm:\n");
    printf("Number Of Page Faults : %d\n", numPageFaults);
    printf("Page Fault Rate: %.2lf%%\n\n", (double)numPageFaults / pageCount * 100);
}

void CLOCK(int* pages, int pageCount, int frameCount)
{
    int frames[MAX_FRAME];
    int useBit[MAX_FRAME];
    int frameStart = 0, frameEnd = 0, useArrow = 0;
    int frameSize = 0;
    int numPageFaults = 0;

    // frames is empty
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    // use bit is empty
    for (int i = 0; i < frameCount; i++)
        useBit[i] = 0;

    for(int i = 0; i < pageCount; i++)
    {
        // check if page exists in frames
        int valid = 0;
        int j;
        for(j = 0; j < frameCount; j++)
        {
            if(pages[i] == frames[j])
            {
                valid = 1;
                break;
            }
        }

        // page already exists in frames
        if(valid)
        {
            useBit[j] = 1;
            continue;
        }
        
        // there are empty frame
        if(frameSize < frameCount)
        {    
            frames[frameEnd] = pages[i];
            frameEnd = (frameEnd + 1) % frameCount;
            frameSize++;
        }
        else
        {
            while(useBit[useArrow] == 1)
            {
                useBit[useArrow] = 0;
                useArrow = (useArrow + 1) % frameCount;
            }
            frames[useArrow] = pages[i];
            useArrow = (useArrow + 1) % frameCount;
        }
        numPageFaults++;        
    }


    printf("Clock Algorithm:\n");
    printf("Number Of Page Faults : %d\n", numPageFaults);
    printf("Page Fault Rate: %.2lf%%\n\n", (double)numPageFaults / pageCount * 100);
}

int main(int argc, char **argv)
{
    FILE *rfile;
    int frameCount;
    int pageCount;
    int pages[5000];
    char buf[10000];
    char *ptr;

    if(argc < 2){
        printf("few arguments\n");
        return -1;
    }
    else if(argc > 2){
        printf("too many arguments\n");
        return -1;
    }

    if((rfile = fopen(argv[1], "r")) == NULL){
        printf("opening file fails\n");
        return -1;
    }
    
    // read the number of frame count
    fgets(buf, sizeof(buf), rfile);
    frameCount = atoi(buf);

    // read page information
    fgets(buf, sizeof(buf), rfile);
    pageCount = strlen(buf) / 2 + 1;
    ptr = strtok(buf, " ");
    for(int i = 0; ptr != NULL; i++)
    {
        pages[i] = atoi(ptr);
        ptr = strtok(NULL, " ");
    }

    OPT(pages, pageCount, frameCount);
    FIFO(pages, pageCount, frameCount);
    LRU(pages, pageCount, frameCount);
    CLOCK(pages, pageCount, frameCount);

    return 0;
}