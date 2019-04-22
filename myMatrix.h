#ifndef MYMATRIX
#define MYMATRIX

/*
Creates a matrix of dimension n x n and initializes it with random
values.
*/
int** createMatrix(int n) {
    int** matrix = malloc(n * sizeof(int*));
    int i,j;
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        matrix[i] = calloc(n, sizeof(int));
        for (j = 0; j < n; j++) {
            matrix[i][j] = ((int)rand() % 20000) - 10000;
        }
    }
    return matrix;
}

/*
Creates a matrix with each value initialized to 0.
*/
int** createEmptyMatrix(int n) {
    int** matrix = malloc(n * sizeof(int*));
    int i;
    for (i = 0; i < n; i++) {
        matrix[i] = calloc(n, sizeof(int));
    }
    return matrix;
}

/*
This function prints the values of a matrix type int. To be
used for matrices where n <= 16.
*/
void printMatrix(int** matrix, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        printf("[");
        for (j = 0; j < n; j++) {
            if (j < n-1) {
                printf("%d,", matrix[i][j]);
            }
            else {
                printf("%d", matrix[i][j]);
            }
        }
        printf("]\n");
    }
}

/*
Cleanup function. Clear the matrices from memory.
*/
void freeMatrix(int** matrix, int n) {
    int i;
    for (i = 0; i < n; i++) free(matrix[i]);

    free(matrix);
}

#endif
