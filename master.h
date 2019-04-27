#ifndef MASTER
#define MASTER

#include "myMatrix.h"

#define COMM_TAG 0
#define STATUS_TAG 1

extern int rank, size, n, k;
extern int **sendBuffer, **compBuffer, statusBuffer;

int **matrixA, **matrixB, **matrixC;

long getMicroTime(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((unsigned long long) time.tv_sec * 1000000) + time.tv_usec;
}

void master() {
    // --------------------------------------------------------------------------------
    // Initialization
    int i;
    long time_start, time_finish;
    
    matrixA = createMatrix(n);
    matrixB = createMatrix(n);
    matrixC = createEmptyMatrix(n);

    time_start = getMicroTime();

    // --------------------------------------------------------------------------------
    // Process
    
    int flag = 0;
    MPI_Status status = NULL;
    MPI_Request reqs[size];
    
    for (i = 0; i < size; i++)
        reqs[i] = NULL;
    
    for (i = 1; i < size; i++) {
        if (reqs[i] != NULL) {
            MPI_Test(&reqs[i], &flag, &status);
            if (flag) {
                reqs = NULL;
            } else {
                continue;
            }
        }
        
        if (status) {
            if (status.MPI_TAG == COMM_TAG) {
            } else if (status.MPI_TAG == STATUS_TAG) {
            }
        }
        
        if (status == NULL) {
            MPI_Irecv(&statusBuffer, 1, MPI_INT, i, STATUS_TAG, MPI_COMM_WORLD, &reqs[i]);
        }
    }
    
    // --------------------------------------------------------------------------------
    // Finish
    time_finish = getMicroTime();
    printf("Time elapsed where n = %d (s): %f\n", n, (double) (time_finish - time_start) / 1000000.0);

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
