#ifndef MASTER
#define MASTER

#define TAG_COMM 0
#define TAG_STATUS 1

#define SLAVE_IDLE 0
#define SLAVE_WORK_COMPLETE 1

#include "myMatrix.h"

extern int rank, size, n, k;
extern int **sendBuffer, **compBuffer, statusBuffer;

long getMicroTime(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((unsigned long long) time.tv_sec * 1000000) + time.tv_usec;
}

void master() {
    // --------------------------------------------------------------------------------
    // Initialization
    int i, a_offset, flag, comp_offset;
    long time_start, time_finish;

    int **matrixA, **matrixB, **matrixC;
    matrixA = createMatrix(n);
    matrixB = createMatrix(n);
    matrixC = createEmptyMatrix(n);

    time_start = getMicroTime();
    a_offset = 0;

    // --------------------------------------------------------------------------------
    // Process

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    for (i = 1; i < size; i++)
    {
        // Send matrixB to all slaves
        MPI_Send(&(B[0][0]), n * n, MPI_INT, i, TAG_COMM, MPI_COMM_WORLD);
    }

    while (a_offset < n)
    {
        flag = 0;

        // probing for 'TAG_STATUS' messages from slaves
        while (!flag)
        {
            for (i = 0; i < 100000; i++); // waiting time, don't poll too often
            MPI_Iprobe(MPI_ANY_SOURCE, TAG_STATUS, MPI_COMM_WORLD, &flag, &status);
        }

        MPI_Recv(&statusBuffer, 1, MPI_INT, MPI_ANY_SOURCE, TAG_STATUS, MPI_COMM_WORLD, &status);

        switch (statusBuffer) {
            case SLAVE_WORK_COMPLETE:
                MPI_Recv(&comp_offset, 1, MPI_INT, status.MPI_SOURCE, TAG_COMM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&(matrixC[comp_offset][0]), k * n, MPI_INT, status.MPI_SOURCE, TAG_COMM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // no-break (fall-through)
            case SLAVE_IDLE:
                // If idle, send work (if work remains)
                if (a_offset < n) {
                    MPI_Send(&a_offset, 1, MPI_INT, status.MPI_SOURCE, TAG_COMM, MPI_COMM_WORLD);
                    MPI_Send(&(A[a_offset][0]), k * n, MPI_INT, status.MPI_SOURCE, TAG_COMM, MPI_COMM_WORLD);
                    a_offset += k;
                }
                break;
        }
    }

    // --------------------------------------------------------------------------------
    // Finish
    time_finish = getMicroTime();
    printf("Time elapsed where n = %d, k = %d (s): %f\n", n, k, (double) (time_finish - time_start) / 1000000.0);

    if (n <= 16) {
        printMatrix(matrixC, n);
    }

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
