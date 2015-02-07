#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "processinfo.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int myRank;
    int processCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    if (myRank == 0) {
        int lowBound = atoi(argv[1]);
        int highBound = atoi(argv[2]);

        if (lowBound < 0) {
            return 1;
        }

        ProcessInfo info;
        buildProcessInfo(&info, myRank, processCount, lowBound, highBound);
        printf("start: %d, end: %d\n", info.start, info.end);
    } else {

    }

    MPI_Finalize();
    return 0;
}


