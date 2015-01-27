#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

void genMatrix(int m, int n, double *A);
void genVector(int n, double *b);

void distributeData(double *A, double *vector, int rowWidth, int processCount, int rowsPerProc, int extraRows) {
    int tag = 0;
    int rowsSent = 0;
    // Skip rows for the leader process
    if (extraRows != 0) {
        rowsSent = rowsPerProc + 1;
        --extraRows;
    }
    for (int i = 1; i < processCount; ++i) {
        // Send an extra row to the first processes to make up for any extras
        int rowsToSend;
        if (extraRows > 0) {
            rowsToSend = rowsPerProc + 1;
            --extraRows;
        } else {
            rowsToSend = rowsPerProc;
        }  
        MPI_Send(A + (rowsSent * rowWidth), rowWidth * rowsToSend, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        rowsSent += rowsToSend;
        MPI_Send(vector, rowWidth, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }
}

void receiveData(double *partialA, double *vector, int rowWidth, int source, int myRank, int rowsPerProc, int extraRows) {
    MPI_Status status;
    
    if (myRank < extraRows) {
        MPI_Recv(partialA, rowWidth * (rowsPerProc + 1), MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    } else {
        MPI_Recv(partialA, rowWidth * rowsPerProc, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Recv(vector, rowWidth, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
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
        
        distributeData(A, b, n, processCount, rowsPerProc, extraRows);
    }
    
    if (myRank != leaderRank) {
        int rowsToReceive;
        if (myRank < extraRows) {
            rowsToReceive = n * (rowsPerProc + 1);
        } else {
            rowsToReceive = n * (rowsPerProc);
        }
        double *partialA = malloc(sizeof(double) * rowsToReceive);
        receiveData(partialA, b, n, leaderRank, myRank, rowsToReceive, extraRows);
        
        free(partialA);
    }

    free(A);
    free(b);
    MPI_Finalize();
    return 0; 
}

