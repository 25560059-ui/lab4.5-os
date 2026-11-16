/*
 * Lab 4 - Exercise 4.5: SJF Scheduling Simulation (Non-preemptive)
 * University of Information Technology (UIT)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void generateProcess(int n, PCB P[]) {
    for (int i = 0; i < n; i++) {
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;      // Arrival range [0, 20]
        P[i].iBurst = 2 + (rand() % 11);  // Burst range [2, 12]
    }
}

void printTable(int n, PCB P[]) {
    printf("PID\tArrival\tBurst\tStart\tFinish\tWaiting\tResponse\tTaT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t%d\t%d\t%d\t%d\t\t%d\n", 
            P[i].iPID, P[i].iArrival, P[i].iBurst, 
            P[i].iStart, P[i].iFinish, P[i].iWaiting, 
            P[i].iResponse, P[i].iTaT);
    }
}

int compareArrival(const void *a, const void *b) {
    PCB *p1 = (PCB *)a;
    PCB *p2 = (PCB *)b;
    if (p1->iArrival != p2->iArrival)
        return p1->iArrival - p2->iArrival;
    return p1->iPID - p2->iPID;
}

int comparePID(const void *a, const void *b) {
    return ((PCB *)a)->iPID - ((PCB *)b)->iPID;
}

void sortReadyQueueSJF(int startIdx, int endIdx, PCB P[]) {
    for (int i = startIdx; i < endIdx - 1; i++) {
        for (int j = i + 1; j < endIdx; j++) {
            if (P[i].iBurst > P[j].iBurst || (P[i].iBurst == P[j].iBurst && P[i].iArrival > P[j].iArrival)) {
                PCB temp = P[i];
                P[i] = P[j];
                P[j] = temp;
            }
        }
    }
}

void runSJF(int testCaseNum) {
    int n = 5;
    PCB Input[10], ReadyQueue[10], TerminatedArray[10];
    generateProcess(n, Input);

    qsort(Input, n, sizeof(PCB), compareArrival);

    int iRemain = n, iReady = 0, iTerminated = 0;

    ReadyQueue[iReady++] = Input[0];
    for (int i = 0; i < iRemain - 1; i++) Input[i] = Input[i + 1];
    iRemain--;

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    while (iTerminated < n) {
        while (iRemain > 0 && Input[0].iArrival <= ReadyQueue[0].iFinish) {
            ReadyQueue[iReady++] = Input[0];
            for (int i = 0; i < iRemain - 1; i++) Input[i] = Input[i + 1];
            iRemain--;
        }

        if (iReady > 2) {
            sortReadyQueueSJF(1, iReady, ReadyQueue);
        }

        TerminatedArray[iTerminated++] = ReadyQueue[0];

        for (int i = 0; i < iReady - 1; i++) {
            ReadyQueue[i] = ReadyQueue[i + 1];
        }
        iReady--;

        if (iReady > 0) {
            if (TerminatedArray[iTerminated - 1].iFinish < ReadyQueue[0].iArrival) {
                ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
            } else {
                ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            }
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        } else if (iRemain > 0) {
            ReadyQueue[iReady++] = Input[0];
            for (int i = 0; i < iRemain - 1; i++) Input[i] = Input[i + 1];
            iRemain--;

            ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }

    qsort(TerminatedArray, n, sizeof(PCB), comparePID);

    printf("\n================ SJF TEST CASE %d ================\n", testCaseNum);
    printTable(n, TerminatedArray);
    
    double totalW = 0, totalTaT = 0;
    for(int i=0; i<n; i++) {
        totalW += TerminatedArray[i].iWaiting;
        totalTaT += TerminatedArray[i].iTaT;
    }
    printf("-> Average Waiting Time (AWT)   = %.2f\n", totalW / n);
    printf("-> Average Turnaround Time (ATaT) = %.2f\n", totalTaT / n);
}

int main() {
    srand(time(1007));
    for(int t = 1; t <= 3; t++) {
        runSJF(t);
    }
    return 0;
}
