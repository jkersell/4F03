#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include <mpi.h>

#include "processinfo.h"
#include "result.h"

void findLargestGap(unsigned int lowBound, unsigned int highBound, Result *result) {
    mpz_t previous;
    mpz_t nextPrime;
    unsigned int largestGap = 0;

    mpz_init_set_ui(previous, lowBound);
    mpz_init(nextPrime);

    while (mpz_get_ui(nextPrime) < highBound) {
        mpz_nextprime(nextPrime, previous);
        mpz_t gap;
        mpz_init(gap);
        mpz_sub(gap, nextPrime, previous);
        if (mpz_cmp_ui(gap, largestGap) > 0) {
            largestGap = mpz_get_ui(gap);
        }
        mpz_set(previous, nextPrime);
    }
    buildResult(result, largestGap, mpz_get_ui(nextPrime));
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int myRank;
    int processCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    const int ROOT = 0;
    if (myRank == ROOT) {
        unsigned int lowBound = atoi(argv[1]);
        unsigned int highBound = atoi(argv[2]);

        if (lowBound < 0) {
            return 1;
        }

        ProcessInfo info;
        buildProcessInfo(&info, myRank, processCount, lowBound, highBound);
        printf("Process 0 start: %d, end: %d\n", info.start, info.end);

        unsigned int send[2] = {lowBound, highBound};
        MPI_Bcast(&send, 2, MPI_UNSIGNED, ROOT, MPI_COMM_WORLD);

        Result *results = malloc(sizeof(Result) * processCount);
        findLargestGap(info.start, info.end, &results[0]);

        send[0] = results[0].largestGap;
        send[1] = results[0].lastPrime;
        unsigned int *recv = malloc(sizeof(unsigned int) * 2 * processCount);
        MPI_Gather(&send, 2, MPI_UNSIGNED, recv, 2, MPI_UNSIGNED, ROOT, MPI_COMM_WORLD);

        unsigned int largestOverall = 0;
        for (int i = 0; i < processCount; ++i) {
            results[i].largestGap = recv[i * 2];
            results[i].lastPrime = recv[(i * 2) + 1];
            printf("Process: %u largestGap: %u lastPrime: %u\n", i, results[i].largestGap, results[i].lastPrime);
            if (largestOverall < results[i].largestGap) {
                largestOverall = results[i].largestGap;
            }
        }
        printf("\n\nLargest gap overall: %lu\n", largestOverall);

        free(recv);
        free(results);
    } else {
        unsigned int recv[2];
        MPI_Bcast(&recv, 2, MPI_INT, ROOT, MPI_COMM_WORLD);

        ProcessInfo info;
        buildProcessInfo(&info, myRank, processCount, recv[0], recv[1]);
        printf("Process %d start: %d, end: %d\n", myRank, info.start, info.end);

        Result result;
        findLargestGap(info.start, info.end, &result);

        unsigned int send[2] = {result.largestGap, result.lastPrime};
        MPI_Gather(&send, 2, MPI_UNSIGNED, NULL, 0, MPI_UNSIGNED, ROOT, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
