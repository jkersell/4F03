#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

typedef struct ProcessInfo {
    int start;
    int end;
} ProcessInfo;

void buildProcessInfo(ProcessInfo *info, int rank, unsigned int processCount, unsigned int globalLow, unsigned int globalHigh);

#endif

