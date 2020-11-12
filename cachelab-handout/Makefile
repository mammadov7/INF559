#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64

all: csim test-rotate tracegen
	# Generate a handin tar file each time you compile
	-tar -cvf ${USER}-handin.tar  csim.c rotate.c 

csim: csim.c cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o csim csim.c cachelab.c -lm 

test-rotate: test-rotate.c rotate.o cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o test-rotate test-rotate.c cachelab.c rotate.o 

tracegen: tracegen.c rotate.o cachelab.c
	$(CC) $(CFLAGS) -O0 -o tracegen tracegen.c rotate.o cachelab.c

rotate.o: rotate.c
	$(CC) $(CFLAGS) -O0 -c rotate.c

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f csim
	rm -f test-rotate tracegen
	rm -f trace.all trace.f*
	rm -f .csim_results .marker
