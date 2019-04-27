#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

int rank, size, n, k;
int **sendBuffer, **compBuffer, statusBuffer;

#include "myMatrix.h"
#include "master.h"
#include "slave.h"

int main(int argc, char** argv) {
	if (argc != 3) {
        printf("usage: %s <n> <k>\n", argv[0]);
        printf("  n: matrix size\n");
        printf("  k: rows per task\n");
        return 1;
    }
    
    n = atoi(argv[1]);
    k = atoi(argv[2]);
    
    sendBuffer = createEmptyMatrix(n);
    compBuffer = createEmptyMatrix(n);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        master();
    } else {
        slave();
    }
    
    freeMatrix(sendBuffer, n);
    freeMatrix(compBuffer, n);

    MPI_Finalize();
    return 0;
}
