/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cachelab.h"
#include <time.h>

rotate_func_t func_list[MAX_ROTATE_FUNCS];
int func_counter = 0; 

/* 
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded. 
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/* 
 * initMatrix - Initialize the given matrix 
 */
void initMatrix(int N, int A[N][N], int B[N][N])
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
            B[j][i]=rand();
        }
    }
}

void randMatrix(int M, int N, int A[N][M]) {
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
        }
    }
}

/* 
 * correctRotate - baseline rotate function used to evaluate correctness 
 */
void correctRotate(int N, int A[N][N], int B[N][N])
{
  int row, col;
  
  for(row=0; row < N; ++row) {
    for(col=0; col < N; ++col) {
      B[col][N-1-row] = A[row][col];
    }
  }
}

/* 
 * registerRotateFunction - Add the given rotate function into your list
 *     of functions to be tested
 */
void registerRotateFunction(void (*rotate)(int N, int[N][N], int[N][N]), 
                           char* desc)
{
    func_list[func_counter].func_ptr = rotate;
    func_list[func_counter].description = desc;
    func_list[func_counter].correct = 0;
    func_list[func_counter].num_hits = 0;
    func_list[func_counter].num_misses = 0;
    func_list[func_counter].num_evictions =0;
    func_counter++;
}
