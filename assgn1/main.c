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

void receiveData(double *partialA, double *vector, int rowWidth, int source, int myRank, int rowsToReceive, int extraRows) {
    MPI_Status status;
    
    MPI_Recv(partialA, rowWidth * rowsToReceive, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(vector, rowWidth, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
}

void vectorProduct(double *partialA, double *vector, int numRows, int rowWidth, double *partialProduct) {
    for (int i = 0; i < numRows; ++i) {
        double dotProduct = 0.0;
        for (int j = 0; j < rowWidth; ++j) {
            dotProduct += *(partialA + (i * rowWidth) + j) * *(vector + j);
        }
        *(partialProduct + i) = dotProduct;
        printf("%f\n", dotProduct);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
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
        printf("\n\n");
        
        distributeData(A, b, n, processCount, rowsPerProc, extraRows);
        
        int rowsOnLeader = rowsPerProc + (extraRows == 0 ? 0 : 1);
        double *finalProduct = malloc(sizeof(double) * m);
        vectorProduct(A, b, rowsOnLeader, n, finalProduct);
        
        int rowsFilled = rowsOnLeader;
        for (int i = 1; i < processCount; ++i) {
            MPI_Status status;
            int rowsToReceive = rowsPerProc + (i < extraRows ? 1 : 0);
            MPI_Recv(finalProduct + rowsFilled, rowsToReceive, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
            rowsFilled += rowsToReceive;
        }
        
        for (int i = 0; i < m; ++i) {
            printf("%f ", *(finalProduct + i));
        }
        printf("\n");
    } else {
        if (myRank < extraRows) {
            ++rowsPerProc;
        }
        double *partialA = malloc(sizeof(double) * n * rowsPerProc);
        receiveData(partialA, b, n, leaderRank, myRank, rowsPerProc, extraRows);
        
        double *partialProduct = malloc(sizeof(double) * rowsPerProc);
        vectorProduct(partialA, b, rowsPerProc, n, partialProduct);
        
        MPI_Send(partialProduct, rowsPerProc, MPI_DOUBLE, leaderRank, 2, MPI_COMM_WORLD);
        
        free(partialA);
    }

    free(A);
    free(b);
    MPI_Finalize();
    return 0; 
}

