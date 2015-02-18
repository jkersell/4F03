#include "processinfo.h"

void buildProcessInfo(ProcessInfo *info, int rank, int processCount, int globalLow, int globalHigh) {
    // Range is inclusive so we need to add 1 here to keep both ends
    int rangeSize = globalHigh - globalLow + 1;
    int bucketSize = rangeSize / processCount;
    int leftover = rangeSize % processCount;

    info->start = globalLow;
    for (int i = 0; i < rank; ++i) {
        info->start += bucketSize;
        if (i < leftover) {
            info->start += 1;
        }
    }
    info->end = info->start + (rank < leftover ? bucketSize : bucketSize - 1);
}
