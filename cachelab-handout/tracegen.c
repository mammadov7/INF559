/* 
 * tracegen.c - Running the binary tracegen with valgrind produces
 * a memory trace of all of the registered rotate functions. 
 * 
 * The beginning and end of each registered rotate function's trace
 * is indicated by reading from "marker" addresses. These two marker
 * addresses are recorded in file for later use.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"
#include <string.h>

/* External variables declared in cachelab.c */
extern rotate_func_t func_list[MAX_ROTATE_FUNCS];
extern int func_counter; 

/* External function from rotate.c */
extern void registerFunctions();

/* Markers used to bound trace regions of interest */
volatile char MARKER_START, MARKER_END;

static int A[256][256];
static int B[256][256];
static int N;


int validate(int fn, int N, int A[N][N], int B[N][N]) {
    int C[N][N];
    memset(C,0,sizeof(C));
    correctRotate(N,A,C);
    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(B[i][j]!=C[i][j]) {
                printf("Validation failed on function %d! Expected %d but got %d at B[%d][%d]\n",fn,C[i][j],B[i][j],i,j);
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char* argv[]){
    int i;

    char c;
    int selectedFunc=-1;
    while( (c=getopt(argc,argv,"N:F:")) != -1){
        switch(c){
        case 'N':
            N = atoi(optarg);
            break;
        case 'F':
            selectedFunc = atoi(optarg);
            break;
        case '?':
        default:
            printf("./tracegen failed to parse its options.\n");
            exit(1);
        }
    }
  

    /*  Register rotate functions */
    registerFunctions();

    /* Fill A with data */
    initMatrix(N, A, B); 

    /* Record marker addresses */
    FILE* marker_fp = fopen(".marker","w");
    assert(marker_fp);
    fprintf(marker_fp, "%llx %llx", 
            (unsigned long long int) &MARKER_START,
            (unsigned long long int) &MARKER_END );
    fclose(marker_fp);

    if (-1==selectedFunc) {
        /* Invoke registered rotate functions */
        for (i=0; i < func_counter; i++) {
            MARKER_START = 33;
            (*func_list[i].func_ptr)(N, A, B);
            MARKER_END = 34;
            if (!validate(i,N,A,B))
                return i+1;
        }
    } else {
        MARKER_START = 33;
        (*func_list[selectedFunc].func_ptr)(N, A, B);
        MARKER_END = 34;
        if (!validate(selectedFunc,N,A,B))
            return selectedFunc+1;

    }
    return 0;
}


