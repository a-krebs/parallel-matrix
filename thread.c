#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

void *slave(void *args) {
	struct threadArgs *localArgs = (struct threadArgs *) args;

#if DEBUG
	printf("Args: %d, %d, size %d\n",
	    localArgs->rStart, localArgs->rEnd, localArgs->size);
#endif

	/* barrier */
	pthread_barrier_wait(&barrier);

	/* do work */
	multiply(
	    localArgs->A, localArgs->B, localArgs->C,
	    localArgs->size, localArgs->rStart, localArgs->rEnd);

#if DEBUG
	printMatrix(localArgs->C, localArgs->size);
#endif

	/* barrier */
	pthread_barrier_wait(&barrier);
	return EXIT_SUCCESS;
}

int getRangeEnd(int rStart, int p, int s) {
	return rStart + s/p - 1;
}
