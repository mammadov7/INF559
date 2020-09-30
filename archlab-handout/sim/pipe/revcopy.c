#include <stdio.h>
#include <stdlib.h>

typedef long word_t;

word_t src[8], dst[8];

/* $begin revcopy */
/*
 * revcopy - copy src to dst in reverse order, returning the
 * number of positive numbers contained in src array.
 */
word_t revcopy(word_t *src, word_t *dst, word_t len)
{
    word_t count = 0;
    word_t val;
    word_t *p_dst = dst+len-1;

    while (len > 0) {
	val = *src++;
	*p_dst-- = val;
	if (val > 0)
	    count++;
	len--;
    }
    return count;
}
/* $end revcopy */

int main()
{
    word_t i, count;

    for (i=0; i<8; i++)
	src[i]= i+1;
    count = revcopy(src, dst, 8);

    for (i=0; i<8; i++)
      printf("%ld ", src[i]);
    printf("\n"); 
    for (i=0; i<8; i++)
      printf("%ld ", dst[i]);
    
    printf ("\ncount=%ld\n", count);
    exit(0);
}


