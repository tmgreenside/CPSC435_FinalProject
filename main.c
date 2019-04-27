#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

int rank, size, n, r;

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

    n = atoi(argv[1]);
    r = atoi(argv[2]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        master();
    } else {
        slave();
    }

    MPI_Finalize();
    return 0;
}
