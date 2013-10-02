#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#define MASTER if (localArgs->id == 0)

/*
 * Thread work function.
 *
 * Each thread (including the master thread) performs this work.
 */
void *slave(void *args) {
	struct threadArgs *localArgs = (struct threadArgs *) args;

#if DEBUG
	printf("ID: %d, start: %d, end: %d, size %d\n",
	    localArgs->id, localArgs->rStart, localArgs->rEnd, localArgs->size);
#endif

	/* barrier */
	pthread_barrier_wait(&barrier);

	MASTER { gettimeofday(&phase2Start, NULL); }

	/* do work */
	multiply(
	    localArgs->A, localArgs->B, localArgs->C,
	    localArgs->size, localArgs->rStart, localArgs->rEnd);

	/* barrier */
	pthread_barrier_wait(&barrier);

	MASTER { gettimeofday(&phase2End); }
	return EXIT_SUCCESS;
}

/*
 * If rStart is the start of the matrix column range (inclusive)
 * return the desired end of the range given matrix size s split
 * between p processors evenly.
 */
int getRangeEnd(int rStart, int p, int s) {
	return rStart + s/p - 1;
}
