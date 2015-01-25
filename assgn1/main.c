#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

void genMatrix(int m, int n, double *A);
void genVector(int n, double *b);

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int m = 10;
    int n = 10;
    double *A = malloc(sizeof(double) * m * n);
    double *b = malloc(sizeof(double) * n);

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

    free(A);
    free(b);
    MPI_Finalize();
    return 0; 
}
