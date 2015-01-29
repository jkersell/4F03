#include <stdlib.h>
#include <time.h>

/*
 * This function fills the block of memory pointed to by A with random doubles.
 * 
 * m, n  The dimensions of the matrix that A represents.
 * A     Points to a block of memory that represents a matrix.
 */
void genMatrix(int m, int n, double *A);

/*
 * This function fills the block of memory pointed to by b with random doubles.
 * 
 * n  The length of the vector that b represents.
 * b  Points to a block of memory that represents a vector.
 */
void genVector(int n, double *b);
