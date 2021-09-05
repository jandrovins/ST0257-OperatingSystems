#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <pthread.h>

typedef struct {
    int rows;
    int cols;
    double** data;
} Matrix;

typedef struct {
    int A_row;
    int B_col;
} RxC;

Matrix A, B, C;
char* isThreadFinishedThreads;
RxC* RxC_arr;

void* mult(void* arg)
{
	int index = (int)arg;
	RxC* RowXCol = &RxC_arr[index];
	double val;
	for(int i = 0; i < A.cols; i++){
		val += A.data[RowXCol->A_row][i] * B.data[i][RowXCol->B_col]; 
	}
	C.data[RowXCol->A_row][RowXCol->B_col] = val;
	isThreadFinishedThreads[RowXCol->A_row * C.cols+RowXCol->B_col] = 1;
	return 0;
}

void initMatrix(int rows, int cols, Matrix* M )
{
    M->rows = rows;
    M->cols = cols;
    // In reality, we will have only one big array and
    // an array of pointers to positions where rows begin on the big array
    // See array2 of http://c-faq.com/aryptr/dynmuldimary.html for a larger explanation.
    M->data = malloc(M->rows * sizeof(double*)); // reserve array of pointers (rows)
    M->data[0] = malloc(M->rows * M->cols * sizeof(double));
    // Define rows that are greater than 0 to be pointers to the array that was allocated to data[0]
    for(int i=1; i<M->rows; i++)
        M->data[i] = M->data[0] + (i * M->cols);
}

void PrintM(Matrix* M)
{
    for(int row=0; row<M->rows; row++ ){
        for(int col=0; col<M->cols; col++){
            printf("%.16lf ", M->data[row][col]);
        }
        printf("\n");
    }
}

void params()
{
    //printf("Ingrese # filas y # columnas de la matriz A\n");
    int numRowsA;
    int numColsA;
    scanf("%d", &numRowsA);
    scanf("%d", &numColsA);
    //printf("%d\n", numRowsA);
    //printf("%d\n", numColsA);

    /************************************/
    /* BEGIN OF DEFINITION OF MATRIX A  */
    initMatrix(numRowsA, numColsA, &A);
    /*  END OF DEFINITION OF MATRICES  */
    /***********************************/

    //printf("Ingrese los valores de A\n");
    for(int i=0; i<numRowsA; i++ ){
        for(int j=0; j<numColsA; j++){
            double val;
            scanf("%lf", &val);
            A.data[i][j] = val; // whatever
        }
    }

    //printf("Ingrese # filas y # columnas de la matriz B\n");
    int numRowsB;
    int numColsB;
    scanf("%d", &numRowsB);
    scanf("%d", &numColsB);

    /************************************/
    /* BEGIN OF DEFINITION OF MATRICES B AND C  */
    initMatrix(numRowsB, numColsB, &B);
    assert (A.cols == B.rows);
    initMatrix(A.rows, B.cols, &C);
    /*  END OF DEFINITION OF MATRICES  */
    /***********************************/

    //printf("Ingrese los valores de B\n");
    for(int i=0; i<numRowsB; i++ ){
        for(int j=0; j<numColsB; j++){
            double val;
            scanf("%lf", &val);
            B.data[i][j] = val; // whatever
        }
    }

    /************************/
    /* BEGIN PRINT MATRICES */
    //printf("A: \n");
    //PrintM(&A);
    //printf("B: \n");
    //PrintM(&B);
    /* BEGIN PRINT MATRICES */
    /************************/
}

int main()
{
	params();

	/****************************/
    /* BEGIN OF THREAD CREATION */
    int NumThreads = C.cols * C.rows;
    isThreadFinishedThreads = calloc(NumThreads, sizeof(char)); // initialized in 0
    pthread_t Threads[NumThreads];
    pthread_attr_t ThreadAttr;
    pthread_attr_init(&ThreadAttr);
    pthread_attr_setdetachstate(&ThreadAttr, PTHREAD_CREATE_DETACHED);
	RxC_arr = calloc(NumThreads, sizeof(RxC));
    for (int r = 0; r < C.rows; r++) {
        for (int c = 0; c < C.cols; c++) {
			int index = c+ (r * C.cols);
            RxC* ThreadMult = &RxC_arr[index];
            ThreadMult->A_row = r;
            ThreadMult->B_col = c;
			pthread_create(&Threads[c + (r * C.cols)], &ThreadAttr, mult, (void *)index);
        }
    }
	free(RxC_arr);
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
			//printf("%d\n", isThreadFinishedThreads[i]);
        }
		//printf("%d\n", threadsFinished);
    }
    /* END CHECKING IF MATRIX MULTIPLICATION IS FINISHED */
    /*****************************************************/

	/************************/
    /* BEGIN PRINT MATRICES */
    //printf("C: \n");
    PrintM(&C);
    /* BEGIN PRINT MATRICES */
    /************************/

	return 0;
}

