#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "cachelab.h"

// Ali MAMMADOV, ali.mammadov

/// Structure of each line in cache
typedef struct Line{
  /// tag
  unsigned long int tag;
  /// validity
  int v;
  /// access time
  int time;
} line;

/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0;         /* set index bits */
int b = 0;         /* block offset bits */
int E = 0;         /* associativity */
char* trace_file = NULL;

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[]){
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/**
 * @brief Finds least recently used line of the set in a cache.
 * @param set of the cache
 * @param E number of lines in set
 * @return index of the line which least recently used 
*/
int LRU( line *set, int E ){
  if ( E == 1 ) return 0;
  int least = set[0].time;
  int i_least = 0;
  for (int i = 1; i < E; i++){
    if ( set[i].time < least ){
      least = set[i].time;
      i_least = i;
    }
  }
  return i_least;
}

/**
 * @brief Prints hits, miss, evicts 
 * @param operation M, S or L
 * @param addr address of block
 * @param size 
*/
void verbose( char operation, unsigned long int addr, int size, int hit, int miss, int evict){
  printf("%c %lx,%d ", operation, addr, size);
  for (int i = 0; i < hit; i++)
    printf("hit ");
  for (int i = 0; i < miss; i++)
    printf("miss ");
  for (int i = 0; i < evict; i++)
    printf("eviction ");
  printf("\n");
}
/*
 * main - Main routine 
 */
int main(int argc, char* argv[])
{
  char c;
  while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
    switch(c){
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      trace_file = optarg;
      break;
    case 'v':
      verbosity = 1;
      break;
    case 'h':
      printUsage(argv);
      exit(0);
    default:
      printUsage(argv);
      exit(1);
    }
  }

  /* Make sure that all required command line args were specified */
  if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
  }
  // number of sets
  unsigned long int nb_sets = (int) pow(2,s);
  // Cashe is 2d array, in which cache[i] is one set, and cache[i][j] is one line of set
  line **cache;
  // Allocation of sets
  cache = (line **)malloc(sizeof(line*)*nb_sets);
  // Allocation of lines of each set
  for (int i = 0; i < nb_sets; i++)
    cache[i] = (line *)malloc(sizeof(line)*E);
  
  // Reading of the trace file

  int miss = 0, hit = 0, evict = 0, time = 0;
  char buffer[32];
  FILE *trace;
  trace = fopen(trace_file,"r");
  //Reads the file till the and, line by line
  while ( fgets( buffer,32, trace ) ){
    // Do nothing, if operation is "I"
    if( buffer[0] == 'I' ) continue;

    char operation; // Operation of trace ( "I","L","S","M" )
    unsigned long int address,tag,set;
    int size;
    // Extracting values from string 
    sscanf(buffer," %c %lx,%d", &operation, &address, &size);
    
    // address = |--tag--|--set--|--block--|, so tag = adress >> (b+nb_sets)
    // set = (adress >> b) & nb_sets -> |0..0|0--0|--set--|
    tag = ( (address >> s) >> b);
    set = (address >> b) & (nb_sets-1);
    // hit, miss and evict for each line, also used for verbose
    int v_hit = 0, v_miss = 0, v_evict = 0;

    // In each line we check if the line is valid or not. 
    // If yes, there is a miss. If not, we check tag of 
    // the line and address' tag. If they are equal,
    // there is hit 
    for (int i = 0; i < E; i++){ 
      time ++; // time for defining when the line was used, for eviction
      if( cache[set][i].v == 0){
        if(operation == 'M')
          v_hit++;
        v_miss ++; 
        cache[set][i].v = 1;
        cache[set][i].tag = tag;
        cache[set][i].time = time;
        break;
      } // end if
      else{
        if( cache[set][i].tag == tag ){
          if( operation == 'M' ) v_hit ++;  // Modification case, one more hit
          v_hit ++; 
          cache[set][i].time = time;
          break;
        } // end if
      } // end else
    } // end for
    
    // If there is nither miss or hit, we should replace the least-recently used line with new one
    if( !v_hit  && !v_miss){
      int i = LRU(cache[set],E);
      if (operation == 'M') v_hit ++; // Modification case, one more hit
      v_evict++;
      v_miss++;
      cache[set][i].v = 1;
      cache[set][i].tag = tag;
      cache[set][i].time = time;
    }
    hit   += v_hit;
    miss  += v_miss;
    evict += v_evict;
    if(verbosity) verbose(operation,address,size,v_hit,v_miss,v_evict);
  } // end while
  fclose(trace); // close file stream  
  // Free space
  for (int i = 0; i < nb_sets; i++) 
    free( cache[i] ); 
  free(cache);
  
  printSummary(hit, miss, evict);
  return 0;
}
