#ifndef MASTER
#define MASTER

#include "myMatrix.h"

int** matrixA, matrixB, matrixC;

long getMicroTime(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((unsigned long long) time.tv_sec * 1000000) + time.tv_usec;
}

void master(int n, int rank, int size) {
    int i;
    long time_start, time_finish;
    
    matrixA = createMatrix(n);
    matrixB = createMatrix(n);
    matrixC = createEmptyMatrix(n);

    time_start = getMicroTime();

    MPI_Request recv = malloc(size * sizeof(MPI_Request));

    for (i = 1; i < size; i++) {
        // TO DO. Send data to Slaves
    }
    
    time_finish = getMicroTime();
    printf("Time elapsed where n = %d (s): %f\n", n, (double) (time_finish - time_start) / 1000000.0);

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
