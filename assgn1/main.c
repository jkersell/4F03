#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

void genMatrix(int m, int n, double *A);
void genVector(int n, double *b);

void distributeMatrix(double *A, int rowWidth, int processCount, int rowsPerProc, int extraRows) {
    int tag = 0;
    for (int i = 1; i < processCount; ++i) {
        // Send an extra row to the first processes to make up for any extras
        if (extraRows > 1) {
            MPI_Send(A + (i * (rowWidth * rowsPerProc)), rowWidth * (rowsPerProc + 1), MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            --extraRows;
        } else {
            MPI_Send(A + (i *(rowWidth * rowsPerProc)), rowWidth * rowsPerProc, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }  
    }
}

void receiveMatrix(double *partialA, int rowWidth, int source, int myRank, int rowsPerProc, int extraRows) {
    MPI_Status status;
    
    if (myRank < extraRows) {
        MPI_Recv(partialA, rowWidth * (rowsPerProc + 1), MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    } else {
        MPI_Recv(partialA, rowWidth * rowsPerProc, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int myRank;
    int processCount;
    int leaderRank = 0;
    int m = 10;
    int n = 10;
    double *A = malloc(sizeof(double) * m * n);
    double *b = malloc(sizeof(double) * n);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    int rowsPerProc = m / processCount;
    int extraRows = m - (rowsPerProc * processCount);
    
    if (myRank == leaderRank) {
        genMatrix(m, n, A);
        genVector(n, b);
        
        distributeMatrix(A, n, processCount, rowsPerProc, extraRows);
        
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                printf("%f ", A[(i * n) + j]);
            }
            printf("\n");
        }

        printf("\n\n");
        
        for (int i = 0; i < n; ++i) {
            printf("%f ", b[i]);
        }
        printf("\n");
    }
    
    if (myRank != leaderRank) {
        double *partialA = NULL;
        if (myRank < extraRows) {
            partialA = malloc(sizeof(double) * n * (rowsPerProc + 1));
        } else {
            partialA = malloc(sizeof(double) * n * (rowsPerProc));
        }
        receiveMatrix(partialA, n, leaderRank, myRank, rowsPerProc, extraRows);
        free(partialA);
    }

    free(A);
    free(b);
    MPI_Finalize();
    return 0; 
}

