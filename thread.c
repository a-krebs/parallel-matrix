#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

void *slave(void *args) {
	struct threadArgs *localArgs = (struct threadArgs *) args;

#if DEBUG
	printf("Args: %d, %d\n", localArgs->rStart, localArgs->rEnd);
#endif

	multiply(
	    localArgs->A, localArgs->B, localArgs->C,
	    localArgs->size, localArgs->rStart, localArgs->rEnd);
	return EXIT_SUCCESS;
}

int getRangeEnd(int rStart, int p, int s) {
	return rStart + s/p - 1;
}
