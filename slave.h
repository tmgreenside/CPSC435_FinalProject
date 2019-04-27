#ifndef SLAVE
#define SLAVE

#include "myMatrix.h"
#include "def.h"

void slave(int rank, int size, int n, int r) {
    int MASTER_RANK = 0;
    MPI_Request request1 = MPI_REQUEST_NULL;
    MPI_Request request2 = MPI_REQUEST_NULL;
    MPI_Request request3 = MPI_REQUEST_NULL;

    int** matrixA = createEmptyMatrix(n);
    int** matrixB = createEmptyMatrix(n);
    int** matrixC = createEmptyMatrix(n);

    int i, j, k, statusBuffer, offset;
    
    //printf("Slave[%i] initialized...\n", rank);

    // receive matrixB from master
    MPI_Irecv(&(matrixB[0][0]), n * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request1);
    
    //printf("Slave[%i] got matrixB from master\n", rank);

    // Send 'SLAVE_IDLE' status (only at beginning, before loop)
    statusBuffer = SLAVE_IDLE;
    MPI_Isend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request2);

    MPI_Wait(&request1, MPI_STATUS_IGNORE);
    MPI_Wait(&request2, MPI_STATUS_IGNORE);
    
    //printf("Slave[%i] sending SLAVE_IDLE, waiting for work...\n", rank);

    while (1) {
        // ------ Receive work
        MPI_Irecv(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_MASTER_STATUS, MPI_COMM_WORLD, &request1);
        MPI_Wait(&request1, MPI_STATUS_IGNORE);

        if (statusBuffer == MASTER_ACK) {
            // Master acknowledge, meaning it will send work
            //printf("Slave[%i] got request to work\n", rank);
        } else if (statusBuffer == MASTER_KILL) {
            // If received 'MASTER_KILL', then there's no more work left to do
            //printf("Slave[%i] got kill signal\n", rank);
            break;
        }

        MPI_Irecv(&offset, 1, MPI_INT, MASTER_RANK, TAG_OFFSET, MPI_COMM_WORLD, &request1);
        MPI_Irecv(&(matrixA[0][0]), r * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request2);
        MPI_Wait(&request1, MPI_STATUS_IGNORE);
        MPI_Wait(&request2, MPI_STATUS_IGNORE);
        
        //printf("Slave[%i] got matrixA rows and offset (%i) from master\n", rank, offset);

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
        
        //printf("Slave[%i] work complete, sending back\n", rank);

        // ------ Work complete, send back
        statusBuffer = SLAVE_WORK_COMPLETE;

        MPI_Isend(&statusBuffer, 1, MPI_INT, MASTER_RANK, TAG_SLAVE_STATUS, MPI_COMM_WORLD, &request1);
        MPI_Isend(&offset, 1, MPI_INT, MASTER_RANK, TAG_OFFSET, MPI_COMM_WORLD, &request2);
        MPI_Isend(&(matrixC[0][0]), r * n, MPI_INT, MASTER_RANK, TAG_MATRIX, MPI_COMM_WORLD, &request3);

        MPI_Wait(&request1, MPI_STATUS_IGNORE);
        MPI_Wait(&request2, MPI_STATUS_IGNORE);
        MPI_Wait(&request3, MPI_STATUS_IGNORE);
        
        //printf("Slave[%i] sent work back, waiting for work...\n", rank);
    }

    //printf("Slave[%i] freeing memory\n", rank);
    // end

    freeMatrix(matrixA, n);
    freeMatrix(matrixB, n);
    freeMatrix(matrixC, n);
    
    //printf("Slave[%i] gracefully exiting\n", rank);
}

#endif
