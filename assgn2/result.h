#ifndef RESULT_H
#define RESULT_H

typedef struct Result {
    unsigned int largestGap;
    unsigned int timeSpent;
} Result;

void buildResult(Result *result, unsigned int largestGap, double timeSpent);

#endif
