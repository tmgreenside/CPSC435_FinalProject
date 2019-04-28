#ifndef MYMATRIX
#define MYMATRIX

/*
Creates a matrix of dimension n x n and initializes it with random
values.
*/
int** createMatrix(int n) {
    int *data = (int *)malloc(n*n*sizeof(int));
    int **array = (int **)malloc(n*sizeof(int*));
    int i,j;
    for (i = 0; i < n; i++) {
        array[i] = &(data[n*i]);
    }
    
    srand(time(NULL));
    
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            array[i][j] = ((int)rand() % 20000) - 10000;
        }
    }
    return array;
}

/*
Creates a matrix with each value initialized to 0.
*/
int** createEmptyMatrix(int n) {
    int *data = (int *)malloc(n*n*sizeof(int));
    int **array = (int **)malloc(n*sizeof(int*));
    int i;
    for (i = 0; i < n; i++) {
        array[i] = &(data[n*i]);
    }
    return array;
}

/*
This function prints the values of a matrix type int. To be
used for matrices where n <= 16.
*/
void printMatrix(int** matrix, int n) {
    int i,j;
    printf("{");
    for (i = 0; i < n; i++)
    {
        if (i ==0)
            printf("{ ");
        else
            printf(" { ");

        for (j = 0; j < n; j++) {
            printf("%6.2f", (float) matrix[i][j]);
            
            if (j < n-1)
                printf(",   ");
        }

        if (i == n-1)
            printf(" }}\n");
        else
            printf(" },\n");
    }
}

/*
Cleanup function. Clear the matrices from memory.
*/
void freeMatrix(int** matrix, int n) {
    free(matrix);
}

#endif
