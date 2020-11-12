/* 
 * cachelab.h - Prototypes for Cache Lab helper functions
 */

#ifndef CACHELAB_TOOLS_H
#define CACHELAB_TOOLS_H

#define MAX_ROTATE_FUNCS 100

typedef struct rotate_func{
  void (*func_ptr)(int N, int[N][N], int[N][N]);
  char* description;
  char correct;
  unsigned int num_hits;
  unsigned int num_misses;
  unsigned int num_evictions;
} rotate_func_t;

/* 
 * printSummary - This function provides a standard way for your cache
 * simulator * to display its final hit and miss statistics
 */ 
void printSummary(int hits,  /* number of  hits */
		  int misses, /* number of misses */
		  int evictions); /* number of evictions */

/* Fill the matrix with data */
void initMatrix(int N, int A[N][N], int B[N][N]);

/* The baseline rotate function that produces correct results. */
void correctRotate(int N, int A[N][N], int B[N][N]);

/* Add the given function to the function list */
void registerRotateFunction(
    void (*rotate)(int N, int[N][N], int[N][N]), char* desc);

#endif /* CACHELAB_TOOLS_H */
