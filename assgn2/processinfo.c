#include "processinfo.h"

void buildProcessInfo(ProcessInfo *info, int rank, int processCount, int globalLow, int globalHigh) {
    int bucketSize = (globalHigh - globalLow) / processCount;
    int leftover = (globalHigh - globalLow) % processCount;

    if (rank < leftover) {
        ++bucketSize;
    }
    info->start = rank * bucketSize;
    info->end = info->start + bucketSize - 1;
}
