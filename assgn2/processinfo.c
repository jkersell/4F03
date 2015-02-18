#include "processinfo.h"

void buildProcessInfo(ProcessInfo *info, int rank, unsigned int processCount, unsigned int globalLow, unsigned int globalHigh) {
    // Range is inclusive so we need to add 1 here to keep both ends
    unsigned int rangeSize = globalHigh - globalLow + 1;
    unsigned int bucketSize = rangeSize / processCount;
    unsigned int leftover = rangeSize % processCount;

    info->start = globalLow;
    for (unsigned int i = 0; i < rank; ++i) {
        info->start += bucketSize;
        if (i < leftover) {
            info->start += 1;
        }
    }
    info->end = info->start + (rank < leftover ? bucketSize : bucketSize - 1);
}
