#ifndef MASTER
#define MASTER

#include "myMatrix.h"

int** matrixA, matrixB, matrixC;

void master(int n, int rank, int size) {
    matrixA = createMatrix(n);
    matrixB = createMatrix(n);
    matrixC = createEmptyMatrix(n);

    struct timeval tpstart, tpend;
    gettimeofday(&tpstart, NULL);

    MPI_Request recv = malloc(size * sizeof(MPI_Request));
    short firstLoop = 1;

    int i;
    for (i = 1; i < size; i++) {
        // TO DO. Send data to Slaves
    }

    gettimeofday(&tpend, NULL);
    double timeUse = (tpend.tv_sec - tpstart.tv_sec) + ((double)(tpend.tv_usec - tpstart.tv_usec)/1000000);
    printf("Time used where n = %d (s): %f\n", n, timeUse);

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
