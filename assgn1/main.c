#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

int m = 10;
int n = 10;

void genMatrix(int m, int n, double *A);
void genVector(int n, double *b);

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    double *A = malloc(sizeof *A * m * n);

    genMatrix(m, n, A);

    for (int i = 1; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%f ", A[i * j]);
        }
        printf("\n");
    }

    free(A);
    MPI_Finalize();
    return 0; 
}
