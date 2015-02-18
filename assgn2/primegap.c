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

    int root = 0;
    if (myRank == root) {
        int lowBound = atoi(argv[1]);
        int highBound = atoi(argv[2]);

        if (lowBound < 0) {
            return 1;
        }

        ProcessInfo info;
        buildProcessInfo(&info, myRank, processCount, lowBound, highBound);
        printf("Process 0 start: %d, end: %d\n", info.start, info.end);

        int send[2] = {lowBound, highBound};
        MPI_Bcast(&send, 2, MPI_INT, root, MPI_COMM_WORLD);
    } else {
        int recv[2];
        MPI_Bcast(&recv, 2, MPI_INT, root, MPI_COMM_WORLD);

        ProcessInfo info;
        buildProcessInfo(&info, myRank, processCount, recv[0], recv[1]);
        printf("Process %d start: %d, end: %d\n", myRank, info.start, info.end);
    }

    MPI_Finalize();
    return 0;
}


