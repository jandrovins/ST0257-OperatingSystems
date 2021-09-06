#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double** data;
} Matrix;

typedef struct {
    int A_row;
    int B_col;
} RxC; // Parameters of threadMult function

Matrix A, B, C;
int8_t* isThreadFinishedThreads;

void* threadMult(void* direction)
{
    RxC* RowXCol = direction;
    double register val;
    for (int i = 0; i < A.cols; i++) {
        val += A.data[RowXCol->A_row][i] * B.data[i][RowXCol->B_col];
    }
    C.data[RowXCol->A_row][RowXCol->B_col] = val;
    isThreadFinishedThreads[RowXCol->A_row * C.cols + RowXCol->B_col] = 1;
    return 0;
}

void initMatrix(int rows, int cols, Matrix* M)
{
    M->rows = rows;
    M->cols = cols;
    // In reality, we will have only one big array and
    // an array of pointers to positions where rows begin on the big array
    // See array2 of http://c-faq.com/aryptr/dynmuldimary.html for a larger explanation.
    M->data = malloc(M->rows * sizeof(double*)); // reserve array of pointers (rows)
    M->data[0] = malloc(M->rows * M->cols * sizeof(double));
    // Define rows that are greater than 0 to be pointers to the array that was allocated to data[0]
    for (int i = 1; i < M->rows; i++)
        M->data[i] = M->data[0] + (i * M->cols);
}

void printMatrix(Matrix* M)
{
    for (int row = 0; row < M->rows; row++) {
        for (int col = 0; col < M->cols; col++) {
            printf("%lf ", M->data[row][col]);
        }
        printf("\n");
    }
}

void readMatrices()
{
    /******************************/
    /* BEGIN OF READING MATRIX A  */
    int numRowsA;
    int numColsA;
    scanf("%d", &numRowsA);
    scanf("%d", &numColsA);
    initMatrix(numRowsA, numColsA, &A);
    for (int i = 0; i < numRowsA; i++) {
        for (int j = 0; j < numColsA; j++) {
            double val;
            scanf("%lf", &val);
            A.data[i][j] = val; // whatever
        }
    }
    /*  END OF READING MATRIX A  */
    /*****************************/

    /**********************/
    /* BEGIN READING OF B */
    int numRowsB;
    int numColsB;
    scanf("%d", &numRowsB);
    scanf("%d", &numColsB);
    initMatrix(numRowsB, numColsB, &B);
    assert(A.cols == B.rows);
    for (int i = 0; i < numRowsB; i++) {
        for (int j = 0; j < numColsB; j++) {
            double val;
            scanf("%lf", &val);
            B.data[i][j] = val; // whatever
        }
    }
    /**********************/
    /*  END READING OF B  */

    /*****************************/
    /* BEGIN INITIALIZATION OF C */
    initMatrix(A.rows, B.cols, &C);
    /* END INITIALIZATION C */
    /************************/
}

int main()
{
    readMatrices();

    /****************************/
    /* BEGIN OF THREAD CREATION */
    int NumThreads = C.cols * C.rows;
    isThreadFinishedThreads = calloc(NumThreads, sizeof(int8_t)); // initialized in 0
    pthread_t Threads[NumThreads];
    pthread_attr_t ThreadAttr;
    pthread_attr_init(&ThreadAttr);
    pthread_attr_setdetachstate(&ThreadAttr, PTHREAD_CREATE_DETACHED);
    RxC RxC_arr[NumThreads];
    for (int r = 0; r < C.rows; r++) {
        for (int c = 0; c < C.cols; c++) {
            int index = c + (r * C.cols);
            RxC_arr[index].A_row = r;
            RxC_arr[index].B_col = c;
            pthread_create(&Threads[index], &ThreadAttr, threadMult, &RxC_arr[index]);
        }
    }
    pthread_attr_destroy(&ThreadAttr);
    /* END OF THREAD CREATION */
    /**************************/

    /*******************************************************/
    /* BEGIN CHECKING IF MATRIX MULTIPLICATION IS FINISHED */
    int threadsFinished = 0;
    while (threadsFinished < NumThreads) {
        threadsFinished = 0;
        for (int i = 0; i < NumThreads; i++) {
            threadsFinished += isThreadFinishedThreads[i];
        }
    }
    /* END CHECKING IF MATRIX MULTIPLICATION IS FINISHED */
    /*****************************************************/

    /**********************/
    /* BEGIN PRINT RESULT */
    printMatrix(&C);
    /* BEGIN PRINT RESULT */
    /**********************/

    return 0;
}
