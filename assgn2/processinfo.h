#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

typedef struct ProcessInfo {
    int start;
    int end;
} ProcessInfo;

void buildProcessInfo(ProcessInfo *info, int rank, int processCount, int globalLow, int globalHigh);

#endif

