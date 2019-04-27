#ifndef SLAVE
#define SLAVE

#include "myMatrix.h"
#include "def.h"

extern int rank, size, n, k;

void slave() {
    int MASTER_RANK = 0;
    MPI_Request request = MPI_REQUEST_NULL;

    int** matrixB = createEmptyMatrix(n);

    int i, j, k, statusBuffer;

    // receive matrixB from master
    MPI_Irecv(&matrixB[i][0], n, MPI_INT, MASTER_RANK, TAG_COMM, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    // Send 'SLAVE_IDLE' status (only at beginning, before loop)
    statusBuffer = SLAVE_IDLE;
    MPI_ISend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    while (1) {
        // Receive master status
        MPI_Irecv(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_MASTER_STATUS, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);

        if (statusBuffer == MASTER_ACK) {
            // Master acknowledge, meaning it will send work
        } else if (statusBuffer == MASTER_KILL) {
            // If received 'MASTER_KILL', then there's no more work left to do
            break;
        }

        // Receive offset here
        // Receive rows here

        // Do multiplication here

        // Send 'SLAVE_WORK_COMPLETE' status
        statusBuffer = SLAVE_WORK_COMPLETE;
        MPI_ISend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);

        // Send offset here
        // Send computed rows here
    }


    // end

    freeMatrix(matrixB, n);
}

#endif
