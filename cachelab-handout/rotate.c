/* 
 * rotate.c - Matrix rotation
 *
 * Each rotation function must have a prototype of the form:
 * void rotate(int N, int A[N][N], int B[N][N]);
 *
 * A rotate function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_rotate(int N, int A[N][N], int B[N][N]);

/* 
 * rotate_submit - This is the solution rotate function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Rotate submission", as the driver
 *     searches for that string to identify the rotate function to
 *     be graded. 
 */
char rotate_submit_desc[] = "Rotate submission";
void rotate_submit(int N, int A[N][N], int B[N][N])
{
  int block = 8;
  int row, col;
  for(row=0; row < N; row += block) {
    for(col=0; col < N; col += block) {
      for (int i = row; i < row+block; i++) {
        for (int j = col; j < col+block; j++){
          B[j][N-1-i] = A[i][j];
        }        
      }
    }
  }

}

/* 
 * You can define additional rotate functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * rotate - A simple baseline rotate function, not optimized for the cache.
 */
char rotate_desc[] = "Simple row-wise scan rotate";
void rotate(int N, int A[N][N], int B[N][N])
{
  int row, col;
  
  for(row=0; row < N; ++row) {
    for(col=0; col < N; ++col) {
      B[col][N-1-row] = A[row][col];
    }
  }
}

/*
 * registerFunctions - This function registers your rotate
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     rotate strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerRotateFunction(rotate_submit, rotate_submit_desc); 

    /* Register any additional rotate functions */
    registerRotateFunction(rotate, rotate_desc); 

}

/* 
 * is_rotate - This helper function checks if B is the rotate of
 *     A. You can check the correctness of your rotate by calling
 *     it before returning from the rotate function.
 */
int is_rotate(int N, int A[N][N], int B[N][N])
{
  int row, col;
  
  for(row=0; row < N; ++row) {
    for(col=0; col < N; ++col) {
      if (B[col][N-1-row] != A[row][col])
	return 0;
    }
  }
  return 1;
}
