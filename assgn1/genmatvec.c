#include <time.h>
#include <stdlib.h>

void genMatrix(int m, int n, double *A) {
    srand48(time(NULL));
    for (int i = 0; i < m * n; ++i) {
        A[i] = drand48();
    }
}

void genVector(int n, double *b) {
}
