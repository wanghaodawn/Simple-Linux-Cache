/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Finished by Hao Wang, Andrew ID: haow2
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB(C) direct mapped cache with a block size of 32 bytes(B).
 */

#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii;
    int temp0, temp1, temp2, temp3, temp4, temp5, temp6, temp7;
    
    REQUIRES(M > 0);
    REQUIRES(N > 0);
    
    if (N == 32 && M == 32) {
        for (i = 0; i < N; i += 8) {
            for (j = 0; j < M; j += 8) {
                for (ii = i; ii < i + 8; ii++) {
                    temp0 = A[ii][j];
                    temp1 = A[ii][j + 1];
                    temp2 = A[ii][j + 2];
                    temp3 = A[ii][j + 3];
                    temp4 = A[ii][j + 4];
                    temp5 = A[ii][j + 5];
                    temp6 = A[ii][j + 6];
                    temp7 = A[ii][j + 7];
                    B[j][ii] = temp0;
                    B[j + 1][ii] = temp1;
                    B[j + 2][ii] = temp2;
                    B[j + 3][ii] = temp3;
                    B[j + 4][ii] = temp4;
                    B[j + 5][ii] = temp5;
                    B[j + 6][ii] = temp6;
                    B[j + 7][ii] = temp7;
                }
            }
        }
    } else if (N == 64 && M == 64) {
        for (i = 0; i < N; i += 4) {
            for (j = 0; j < M; j += 4) {
                for (ii = i; ii < i + 4; ii++) {
                    temp0 = A[ii][j];
                    temp1 = A[ii][j + 1];
                    temp2 = A[ii][j + 2];
                    temp3 = A[ii][j + 3];
                    B[j][ii] = temp0;
                    B[j + 1][ii] = temp1;
                    B[j + 2][ii] = temp2;
                    B[j + 3][ii] = temp3;
                }
            }
        }
    } else if(N == 67 && M == 61) {
        for (i = 0; i < 60; i += 4) {
            for (j = 0; j < 60; j += 4) {
                for (ii = i; ii < i + 4; ii++) {
                    temp0 = A[ii][j];
                    temp1 = A[ii][j + 1];
                    temp2 = A[ii][j + 2];
                    temp3 = A[ii][j + 3];
                    B[j][ii] = temp0;
                    B[j + 1][ii] = temp1;
                    B[j + 2][ii] = temp2;
                    B[j + 3][ii] = temp3;
                }
            }
        }
        for (j = 0; j < 60; j++) {
            temp0 = A[60][j];
            temp1 = A[61][j];
            temp2 = A[62][j];
            temp3 = A[63][j];
            temp4 = A[64][j];
            temp5 = A[65][j];
            temp6 = A[66][j];
            B[j][60] = temp0;
            B[j][61] = temp1;
            B[j][62] = temp2;
            B[j][63] = temp3;
            B[j][64] = temp4;
            B[j][65] = temp5;
            B[j][66] = temp6;
        }
        for (i = 0; i < 64; i += 8) {
            temp0 = A[i][60];
            temp1 = A[i + 1][60];
            temp2 = A[i + 2][60];
            temp3 = A[i + 3][60];
            temp4 = A[i + 4][60];
            temp5 = A[i + 5][60];
            temp6 = A[i + 6][60];
            temp7 = A[i + 7][60];
            B[60][i] = temp0;
            B[60][i + 1] = temp1;
            B[60][i + 2] = temp2;
            B[60][i + 3] = temp3;
            B[60][i + 4] = temp4;
            B[60][i + 5] = temp5;
            B[60][i + 6] = temp6;
            B[60][i + 7] = temp7;
        }
        temp0 = A[64][60];
        B[60][64] = temp0;
        temp0 = A[65][60];
        B[60][65] = temp0;
        temp0 = A[66][60];
        B[60][66] = temp0;
    }
    
    ENSURES(is_transpose(M, N, A, B));
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

