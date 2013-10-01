/*
 * Parallel implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "shared.h"
#include "thread.h"

/* shared memory */
int **A = NULL;
int **B = NULL;
int **C = NULL;
int Size = 0;

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
	
	args = calloc(1, sizeof(struct arguments));
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	Size = args->size;
	numThreads = args->procs -1;	/* -1 to account for master */
	seed = args->seed;

	/* allocate memory for matrices */
	A = allocMatrixInt(Size);
	B = allocMatrixInt(Size);
	C = allocMatrixInt(Size);

	/* initialize matrix contents */
	srandom(seed);
	initMatrixInt(A, Size);
	initMatrixInt(B, Size);

	/* allocate memory for threads */
	threads = calloc(numThreads, sizeof(pthread_t));
	threadArgs = calloc(numThreads, sizeof(struct threadArgs));

	/* split work between threads */
	for (i = 0; i < numThreads; i++) {
		rEnd = getRangeEnd(rStart, args->procs, Size);
		threadArgs[i].rStart = rStart;
		threadArgs[i].rEnd = rEnd;
		rStart = rEnd + 1;

		threadArgs[i].A = A;
		threadArgs[i].B = B;
		threadArgs[i].C = C;
	}
	/* set rEnd for master thread */
	rEnd = getRangeEnd(rStart, args->procs, Size);

	/* Spin up threads */
	for (i = 0; i < numThreads; i++) {
		pthread_create(&threads[i], NULL, slave, &threadArgs[i]);
	}

	/* barrier */
	// TODO
	
#if VERIFY
	printMatrix(A, Size);
	printMatrix(B, Size);
#else
	printTOD();
#endif

#if DEBUG
	printf("Master args: %d, %d\n", rStart, rEnd);
#endif
	/* master thread work */
	multiply(A, B, C, Size, rStart, rEnd);

	/* join threads */
	for (i = 0; i < numThreads; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Pthread_join fails");
		}
	}
#if VERIFY
	printMatrix(C, Size);
#else
	printTOD();
#endif

	freeMatrixInt(A, Size);
	freeMatrixInt(B, Size);
	freeMatrixInt(C, Size);
	free(args);
	free(threads);
	free(threadArgs);
	exit(EXIT_SUCCESS);
}
