#ifndef SLAVE
#define SLAVE

#include "myMatrix.h"
#include "def.h"

extern int rank, size, n, r;

void slave() {
    int MASTER_RANK = 0;
    MPI_Request request1 = MPI_REQUEST_NULL;
    MPI_Request request2 = MPI_REQUEST_NULL;
    MPI_Request request3 = MPI_REQUEST_NULL;

    int** matrixA = createEmptyMatrix(n);
    int** matrixB = createEmptyMatrix(n);
    int** matrixC = createEmptyMatrix(n);

    int i, j, k, statusBuffer, offset;

    // receive matrixB from master
    MPI_Irecv(&(matrixB[0][0]), n * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request1);

    // Send 'SLAVE_IDLE' status (only at beginning, before loop)
    statusBuffer = SLAVE_IDLE;
    MPI_Isend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request2);

    MPI_Wait(&request1, MPI_STATUS_IGNORE);
    MPI_Wait(&request2, MPI_STATUS_IGNORE);

    while (1) {
        // ------ Receive work
        MPI_Irecv(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_MASTER_STATUS, MPI_COMM_WORLD, &request1);
        MPI_Wait(&request1, MPI_STATUS_IGNORE);

        if (statusBuffer == MASTER_ACK) {
            // Master acknowledge, meaning it will send work
        } else if (statusBuffer == MASTER_KILL) {
            // If received 'MASTER_KILL', then there's no more work left to do
            break;
        }

        MPI_Irecv(&offset, 1, MPI_INT, MASTER_RANK, TAG_OFFSET, MPI_COMM_WORLD, &request1);
        MPI_Irecv(&(matrixA[0][0]), r * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request2);
        MPI_Wait(&request1, MPI_STATUS_IGNORE);
        MPI_Wait(&request2, MPI_STATUS_IGNORE);

        // ------ Do work (multiplication)
        for (k = 0; k < n; k++)
        {
            for (i = 0; i < r; i++)
            {
                matrixC[i][k] = 0;
                for (j = 0; j < n; j++)
                    matrixC[i][k] += matrixA[i][j] * matrixB[j][k];
            }
        }

        // ------ Work complete, send back
        statusBuffer = SLAVE_WORK_COMPLETE;

        MPI_Isend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request1);
        MPI_Isend(&offset, 1, MPI_INT, MASTER_RANK, TAG_OFFSET, MPI_COMM_WORLD, &request2);
        MPI_Isend(&(matrixC[0][0]), r * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request3);

        MPI_Wait(&request1, MPI_STATUS_IGNORE);
        MPI_Wait(&request2, MPI_STATUS_IGNORE);
        MPI_Wait(&request3, MPI_STATUS_IGNORE);
    }


    // end

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
}

#endif
