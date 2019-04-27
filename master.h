#ifndef MASTER
#define MASTER

#include "myMatrix.h"
#include "def.h"

extern int rank, size, n, r;

long getMicroTime(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((unsigned long long) time.tv_sec * 1000000) + time.tv_usec;
}

void master() {
    // --------------------------------------------------------------------------------
    // Initialization
    int i, a_offset, flag, comp_offset, statusBuffer, slaves_alive;
    long time_start, time_finish;

    int **matrixA, **matrixB, **matrixC;
    matrixA = createMatrix(n);
    matrixB = createMatrix(n);
    matrixC = createEmptyMatrix(n);

    time_start = getMicroTime();
    a_offset = 0;
    slaves_alive = size - 1;

    // --------------------------------------------------------------------------------
    // Process

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    for (i = 1; i < size; i++)
    {
        // Send matrixB to all slaves
        MPI_Send(&(matrixB[0][0]), n * n, MPI_INT, i, TAG_MATRIX, MPI_COMM_WORLD);
    }

    while (slaves_alive > 0)
    {
        flag = 0;

        // probing for 'TAG_SLAVE_STATUS' messages from slaves
        while (!flag)
        {
            for (i = 0; i < 100000; i++); // waiting time, don't poll too often
            MPI_Iprobe(MPI_ANY_SOURCE, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &flag, &status);
        }

        MPI_Recv(&statusBuffer, 1, MPI_INT, MPI_ANY_SOURCE, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &status);

        switch (statusBuffer) {
            case SLAVE_WORK_COMPLETE:
                MPI_Recv(&comp_offset, 1, MPI_INT, status.MPI_SOURCE, TAG_OFFSET, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&(matrixC[comp_offset][0]), r * n, MPI_INT, status.MPI_SOURCE, TAG_MATRIX, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // no-break (fall-through)
            case SLAVE_IDLE:
                // If idle, send work (if work remains)
                if (a_offset < (n - 1)) {
                    // Send 'MASTER_ACK' status
                    statusBuffer = MASTER_ACK;
                    MPI_Send(&statusBuffer, 1, MPI_INT, status.MPI_SOURCE, TAG_MASTER_STATUS, MPI_COMM_WORLD);
                    
                    MPI_Send(&a_offset, 1, MPI_INT, status.MPI_SOURCE, TAG_OFFSET, MPI_COMM_WORLD);
                    MPI_Send(&(matrixA[a_offset][0]), r * n, MPI_INT, status.MPI_SOURCE, TAG_MATRIX, MPI_COMM_WORLD);
                    a_offset += r;
                } else {
                    // Send 'MASTER_KILL' status
                    statusBuffer = MASTER_KILL;
                    MPI_Send(&statusBuffer, 1, MPI_INT, status.MPI_SOURCE, TAG_MASTER_STATUS, MPI_COMM_WORLD);
                    slaves_alive--;
                }
                break;
        }
    }
    

    // --------------------------------------------------------------------------------
    // Finish
    time_finish = getMicroTime();
    printf("Time elapsed where n = %d, r = %d (s): %f\n", n, r, (double) (time_finish - time_start) / 1000000.0);

    if (n <= 16) {
        printMatrix(matrixC, n);
    }

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
