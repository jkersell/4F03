#include <time.h>
#include <stdlib.h>

/*
 * This function fills the block of memory pointed to by A with random doubles.
 * 
 * m, n  The dimensions of the matrix that A represents.
 * A     Points to a block of memory that represents a matrix.
 */
void genMatrix(int m, int n, double *A) {
    srand48(time(NULL));
    for (int i = 0; i < m * n; ++i) {
        A[i] = drand48();
    }
}

/*
 * This function fills the block of memory pointed to by b with random doubles.
 * 
 * n  The length of the vector that b represents.
 * b  Points to a block of memory that represents a vector.
 */
void genVector(int n, double *b) {
    srand48(time(NULL));
    for (int i = 0; i < n; ++i) {
        b[i] = drand48();
    }
}
