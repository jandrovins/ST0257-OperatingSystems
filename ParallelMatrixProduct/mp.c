#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <assert.h>
#include <pthread.h>


void mult(LxR* arg)
{
}

typedef struct {
    int rows;
    int cols;
    double** data;
} Matrix;
typedef struct {
    int A_row;
    int B_col;
} RxC;

void initMatrix(int rows, int cols, Matrix* M);
void initWhateverInAB(); // only for testing
void PrintM(Matrix* M);

Matrix A, B, C;
char* isThreadFinishedThreads;
int main()
{
    /************************************/
    /* BEGIN OF DEFINITION OF MATRICES  */
    initMatrix(3, 3, &A);
    initMatrix(3, 4, &B);
    assert(A.cols == B.rows);
    initMatrix(A.rows, B.cols, &C);
    /*  END OF DEFINITION OF MATRICES  */
    /***********************************/

    /***********************************/
    /* BEGIN INITIALIZATION OF A AND B */
    initWhateverInAB();
    /* END INITIALIZATION OF A AND B */
    /*********************************/

    /****************************/
    /* BEGIN OF THREAD CREATION */
    int NumThreads = C.cols * C.rows;
    isThreadFinishedThreads = calloc(NumThreads, sizeof(char)); // initialized in 0
    pthread_t Threads[NumThreads];
    pthread_attr_t ThreadAttr;
    pthread_attr_init(&ThreadAttr);
    pthread_attr_setdetachstate(&ThreadAttr, PTHREAD_CREATE_DETACHED);
    for (int r = 0; r < C.rows; r++) {
        for (int c = 0; c < C.cols; c++) {
            RxC* ThreadMult = malloc(sizeof(RxC));
            ThreadMult->A_row = r;
            ThreadMult->B_col = c;
            pthread_create(Threads[c + (r * C.cols)], ThreadAttr, mult, ThreadMult);
        }
    }
    pthread_attr_destroy(&ThreadAttr);
    /* END OF THREAD CREATION */
    /**************************/

    /*******************************************************/
    /* BEGIN CHECKING IF MATRIX MULTIPLICATION IS FINISHED */
    int threadsFinished = 0;
    while(threadsFinished < NumThreads){
        threadsFinished = 0;
        for(int i=0; i < NumThreads; i++){
            threadsFinished += isThreadFinishedThreads[i];
        }
    }
    /* END CHECKING IF MATRIX MULTIPLICATION IS FINISHED */
    /*****************************************************/

    /**********************/
    /* BEGIN PRINT RESULT */
    printf("C: \n");
    PrintM(&C);
    /********************/
    /* END PRINT RESULT */
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
void initWhateverInAB()
{ // only for testing
    for (int r = 0; r < A.rows; r++)
        for (int c = 0; c < A.cols; c++) {
            A.data[r][c] = r + c; // whatever
        }
    for (int r = 0; r < B.rows; r++)
        for (int c = 0; c < B.cols; c++) {
            B.data[r][c] = r + c; // whatever second time
        }
}
void PrintM(Matrix* M)
{
    for (int row = 0; row < M->rows; row++) {
        for (int col = 0; col < M->cols; col++) {
            printf("%lf ", M->data[row][col]);
        }
        printf("\n");
    }
}
