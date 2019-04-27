#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include "myMatrix.h"
#include "master.h"
#include "slave.h"
#include "def.h"

int main(int argc, char** argv) {
	if (argc != 3) {
        printf("usage: %s <n> <r>\n", argv[0]);
        printf("  n: matrix size\n");
        printf("  r: rows per task\n");
        return 1;
    }
    
    int rank, size, n, r;

    n = atoi(argv[1]);
    r = atoi(argv[2]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //printf("Initialized: rank=%i/%i\n", rank, size-1);

    if (rank == 0) {
        master(rank, size, n, r);
    } else {
        slave(rank, size, n, r);
    }

    //printf("Terminated: rank=%i/%i\n", rank, size-1);
    MPI_Finalize();
    return 0;
}
