#ifndef RESULT_H
#define RESULT_H

typedef struct Result {
    unsigned int largestGap;
    double timeSpent;
} Result;

void buildResult(Result *result, unsigned int largestGap, double timeSpent);

#endif
