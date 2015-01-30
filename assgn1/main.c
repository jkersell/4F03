#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#include "genmatvec.h"
#include "matvecres.h"

/* This function is part c.
 * This function distributes A and b across all processes.
 *
 * A  The matrix to distribute represented as a one dimentional array.
 * vector  The vector to distribute.
 * rowWidth  The length of each row.
 * processCount  The number of processes to distribute to.
 * rowsPerProc  The number of rows to send to each process.
 * extraRows  The number of rows that could not be evenly divided across processes.
 */
void distributeData(double *A, double *vector, int rowWidth, int processCount, int rowsPerProc, int extraRows) {
    int tag = 0;
    int rowsSent = rowsPerProc;
    // Skip rows for the leader process
    if (extraRows != 0) {
        ++rowsSent;
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
        MPI_Send(&A[rowsSent * rowWidth], rowWidth * rowsToSend, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        rowsSent += rowsToSend;
    }
    MPI_Bcast(vector, rowWidth, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

/*
 * This is also part c
 * This function receives part of A and all of b from the leader process.
 *
 * partialA  Output. A pointer to the memory to store this process' part of A in.
 * vector  Output. A pointer to the memort to store this process' copy of b in.
 * rowWidth  The length of each row.
 * rowsToReceive  the number of rows to receive on this process.
 */
void receiveData(double *partialA, double *vector, int rowWidth, int rowsToReceive) {
    MPI_Status status;
    
    MPI_Recv(partialA, rowWidth * rowsToReceive, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Bcast(vector, rowWidth, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

/*
 * This function is part d.
 * This function computes the partial matrix product of A and b.
 *
 * partialA  A pointer to the memory to store this process' part of A in.
 * vector  A pointer to the memort to store this process' copy of b in.
 * numRows  The number of rows in partialA.
 * rowWidth  The length of each row.
 * partialProduct  Output. The part of the product computer on this process.
 */
void vectorProduct(double *partialA, double *vector, int numRows, int rowWidth, double *partialProduct) {
    for (int i = 0; i < numRows; ++i) {
        double dotProduct = 0.0;
        for (int j = 0; j < rowWidth; ++j) {
            dotProduct += partialA[(i * rowWidth) + j] * vector[j];
        }
        partialProduct[i] = dotProduct;
    }
}

/*
 * This function is part of part f.
 * This function outputs an Octave program which can be piped into Octave to verify the result.
 *
 * A  The matrix.
 * b  The vector.
 * result  The result of the computation.
 * m  The number of rows in A.
 * n  The number of columns in A.
 */
void printForOctave(double *A, double *b, double *result, int m, int n) {
    printf("A = [");
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%.*f", DBL_DIG, A[(i * n) + j]);
            if (j != n - 1) {
                printf(" ");
            }
        }
        if (i != m - 1) {
            printf(";\n");
        }
    }
    printf("];\n");
    
    printf("b = [");
    for (int i = 0; i < n; ++i) {
        printf("%.*f", DBL_DIG, b[i]);
        if (i != n - 1) {
            printf("; ");
        }
    }
    printf("];\n");
    
    printf("result = [");
    for (int i = 0; i < m; ++i) {
        printf("%.*f", DBL_DIG, result[i]);
        if (i != m - 1) {
            printf("; ");
        }
    }
    printf("];\n");
    
    printf("octaveResult = A*b;\n");
    printf("errorThreshold = 1.0e-12;\n");
    printf("diff = abs(octaveResult - result);\n");
    printf("passed = all(diff < errorThreshold);\n");
    printf("save octave.result A b octaveResult diff errorThreshold passed;\n");
    printf("if (passed) printf(\"Test passed!\\n\") else printf(\"Test Failed\\n\") endif\n");
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int myRank;
    int processCount;
    int leaderRank = 0;
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    if (m <= 0 || n <= 0) {
        return 1;
    }
    double *b = malloc(sizeof(double) * n);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    int rowsPerProc = m / processCount;
    int extraRows = m - (rowsPerProc * processCount);
    
    if (myRank == leaderRank) {
        double *A = malloc(sizeof(double) * m * n);
        genMatrix(m, n, A);
        genVector(n, b);
        
        distributeData(A, b, n, processCount, rowsPerProc, extraRows);
        
        int rowsOnLeader = rowsPerProc + (extraRows == 0 ? 0 : 1);
        double *finalProduct = malloc(sizeof(double) * m);
        vectorProduct(A, b, rowsOnLeader, n, finalProduct);
        
        // part e
        int rowsFilled = rowsOnLeader;
        for (int i = 1; i < processCount; ++i) {
            MPI_Status status;
            int rowsToReceive = rowsPerProc + (i < extraRows ? 1 : 0);
            MPI_Recv(finalProduct + rowsFilled, rowsToReceive, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
            rowsFilled += rowsToReceive;
        }
        
        printForOctave(A, b, finalProduct, m, n);
        getResult(m, n, A, b, finalProduct);
        
        free(finalProduct);
        free(A);
    } else {
        if (myRank < extraRows) {
            ++rowsPerProc;
        }
        double *partialA = malloc(sizeof(double) * n * rowsPerProc);
        receiveData(partialA, b, n, rowsPerProc);
        
        double *partialProduct = malloc(sizeof(double) * rowsPerProc);
        vectorProduct(partialA, b, rowsPerProc, n, partialProduct);
        
        MPI_Send(partialProduct, rowsPerProc, MPI_DOUBLE, leaderRank, 2, MPI_COMM_WORLD);
        
        free(partialA);
    }

    free(b);
    MPI_Finalize();
    return 0; 
}

