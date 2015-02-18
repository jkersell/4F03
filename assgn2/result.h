#ifndef RESULT_H
#define RESULT_H

typedef struct Result {
    unsigned int largestGap;
    unsigned int lastPrime;
} Result;

void buildResult(Result *result, unsigned int largestGap, unsigned int lastPrime);

#endif
