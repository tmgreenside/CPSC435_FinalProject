#ifndef SLAVE
#define SLAVE

#include "myMatrix.h"
#include "def.h"

extern int rank, size, n, k;

void slave() {
    int** matrixB = calloc(n, sizeof(int*));
    int i, j, k;
    for (i = 0; i < n; i++) matrixB[i] = malloc(n * sizeof(int));

    // receive matrixB from master
    for (i = 0; i < n; i++) {
        MPI_Irecv(&matrixB[i][0], n, MPI_INT, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Get work, process

    

    // end

    freeMatrix(matrixB, n);
}

#endif
