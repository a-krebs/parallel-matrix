/*
 * Parallel implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "shared.h"
#include "thread.h"

int main(int argc, char *argv[]) {
	int i = 0;
	int argsParsed = 0;
	struct arguments *args = NULL;
	pthread_t *threads = NULL;
	struct threadArgs *threadArgs = NULL;
	int numThreads = 0;
	int rStart = 0;
	int rEnd = 0;
	unsigned int seed = 0;
	int **A = NULL;
	int **B = NULL;
	int **C = NULL;
	int size = 0;
	
	args = calloc(1, sizeof(struct arguments));
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	size = args->size;
	numThreads = args->procs -1;	/* -1 to account for master */
	seed = args->seed;

	/* allocate memory for matrices */
	A = allocMatrixInt(size);
	B = allocMatrixInt(size);
	C = allocMatrixInt(size);

	/* initialize matrix contents */
	srandom(seed);
	initMatrixInt(A, size);
	initMatrixInt(B, size);

	/* allocate memory for threads */
	threads = calloc(numThreads, sizeof(pthread_t));
	threadArgs = calloc(numThreads, sizeof(struct threadArgs));

	/* split work between threads */
	for (i = 0; i < numThreads; i++) {
		rEnd = getRangeEnd(rStart, args->procs, size);
		threadArgs[i].rStart = rStart;
		threadArgs[i].rEnd = rEnd;
		rStart = rEnd + 1;

		/* set remaining thread arguments */
		threadArgs[i].A = A;
		threadArgs[i].B = B;
		threadArgs[i].C = C;
		threadArgs[i].size = size;
	}
	/* set rEnd for master thread */
	rEnd = getRangeEnd(rStart, args->procs, size);

	/* Spin up threads */
	for (i = 0; i < numThreads; i++) {
		pthread_create(&threads[i], NULL, slave, &threadArgs[i]);
	}

	/* barrier */
	// TODO
	
#if VERIFY
	printMatrix(A, size);
	printMatrix(B, size);
#else
	printTOD();
#endif

#if DEBUG
	printf("Master args: %d, %d\n", rStart, rEnd);
#endif
	/* master thread work */
	multiply(A, B, C, size, rStart, rEnd);

	/* join threads */
	for (i = 0; i < numThreads; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Pthread_join fails");
		}
	}
#if VERIFY
	printMatrix(C, size);
#else
	printTOD();
#endif

	freeMatrixInt(A, size);
	freeMatrixInt(B, size);
	freeMatrixInt(C, size);
	free(args);
	free(threads);
	free(threadArgs);
	exit(EXIT_SUCCESS);
}
