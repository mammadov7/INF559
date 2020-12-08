/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Mammadov",
    /* First member's full name */
    "Ali Mammadov",
    /* First member's email address */
    "ali.mammadov@ip-paris.fr",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* Constants and macros from Textbook provided in malloclab intro video */
#define WSIZE       4       /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE       8       /* Doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)                    //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      //line:vm:mm:hdrp
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous free blocks*/
#define NEXT_FREE(bp)(*(void **)(bp))
#define PREV_FREE(bp)(*(void **)(bp + WSIZE))

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //line:vm:mm:nextblkp
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //line:vm:mm:prevblkp

/* single word (4) or double word (8) alignment */
#define ALIGN4(size) (((size) + (4-1)) & ~0x7)

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

static void *extend_heap(size_t size);
static void remove_freeblock(void *bp);
static void *coalesce(void *bp);
static void place(void *bp, size_t asize);

static char *free_lists = 0;  /* Poitns to the frist free block */	

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	char *heap_start = 0;	
	int block_size = 2*DSIZE;
  if ((heap_start = mem_sbrk(4*DSIZE)) == (void *)-1)
      return -1; 
	/*Prolog Header*/
  PUT(heap_start,             PACK(block_size, 1));        
	/* Free block header */   
  PUT(heap_start +    WSIZE,  PACK(block_size, 0));            
	/*Payload*/
  PUT(heap_start + (2*WSIZE), PACK(0,0));
  PUT(heap_start + (3*WSIZE), PACK(0,0));
	/* Free block footer */  
  PUT(heap_start + (4*WSIZE), PACK(block_size, 0));
	/* New epilogue header */
  PUT(heap_start + (5*WSIZE), PACK(0, 1));                      
	/* Point to the first free block*/
  free_lists = heap_start + (WSIZE);
  return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

	if (size == 0)
  	return NULL;

  size_t newsize = MAX(ALIGN(size) + DSIZE, 2*DSIZE); 
  char *bp;
  
  // Search the free list for the fit 
  for (bp = free_lists; GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREE(bp)) 
    if (newsize <= GET_SIZE(HDRP(bp))){
    	place(bp, newsize);
		  return bp; 
		}
 
  // Otherwise, no fit was found. Grow the heap larger. 
  if ((bp = extend_heap(newsize)) == NULL)
    return NULL;

  // Place the newly allocated block
  place(bp, newsize);
  return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{ 
  
  // Ignore spurious requests 
  if (!bp)
      return;

  size_t size = GET_SIZE(HDRP(bp));

  /* Set the header and footer allocated bits to 0, thus
   * freeing the block */
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));

  // Coalesce to merge any free blocks and add them to the list 
  coalesce(bp);
}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


static void *extend_heap(size_t size)
{
  char *bp;
  size_t asize;

  /* Adjust the size so the alignment and minimum block size requirements
   * are met. */ 
  asize = MAX(ALIGN4(size),2*DSIZE);
  
  // Attempt to grow the heap by the adjusted size 
  if ((bp = mem_sbrk(asize)) == (void *)-1)
    return NULL;

	/* Free block header */ 
 	PUT(HDRP(bp), PACK(asize, 0));
	/* Free block footer */
  PUT(FTRP(bp), PACK(asize, 0));
	/* Epilogue header */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); 


  return coalesce(bp); 
}


static void *coalesce(void *bp)
{
  // Determine the current allocation state of the previous and next blocks 
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));

  // Get the size of the current free block
  size_t size = GET_SIZE(HDRP(bp));

  /* If the next block is free, then coalesce the current block
   * (bp) and the next block */
  if (prev_alloc && !next_alloc) {           // Case 2 
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));  
    remove_freeblock(NEXT_BLKP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }

  /* If the previous block is free, then coalesce the current
   * block (bp) and the previous block */
  else if (!prev_alloc && next_alloc) {      // Case 3 
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    bp = PREV_BLKP(bp); 
    remove_freeblock(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  } 

  /* If the previous block and next block are free, coalesce
   * both */
  else if (!prev_alloc && !next_alloc) {     // Case 4
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
            GET_SIZE(HDRP(NEXT_BLKP(bp)));
    remove_freeblock(PREV_BLKP(bp));
    remove_freeblock(NEXT_BLKP(bp));
    bp = PREV_BLKP(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }

  // Insert the coalesced block at the front of the free list 
  NEXT_FREE(bp) = free_lists;
  PREV_FREE(free_lists) = bp;
  PREV_FREE(bp) = NULL;
  free_lists = bp;

  // Return the coalesced block 
  return bp;
}

static void remove_freeblock(void *bp)
{
  if(bp) {
    if (PREV_FREE(bp))
      NEXT_FREE(PREV_FREE(bp)) = NEXT_FREE(bp);
    else
      free_lists = NEXT_FREE(bp);
    if(NEXT_FREE(bp) != NULL)
      PREV_FREE(NEXT_FREE(bp)) = PREV_FREE(bp);
  }
}

static void place(void *bp, size_t asize)
{  
  // Gets the total size of the free block 
  size_t fsize = GET_SIZE(HDRP(bp));
  // Case 1: Splitting is performed 
  if((fsize - asize) >= (2*DSIZE)) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    remove_freeblock(bp);
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(fsize-asize, 0));
    PUT(FTRP(bp), PACK(fsize-asize, 0));
    coalesce(bp);
  }
  // Case 2: Splitting not possible. Use the full free block 
  else {
    PUT(HDRP(bp), PACK(fsize, 1));
    PUT(FTRP(bp), PACK(fsize, 1));
    remove_freeblock(bp);
  }
}
