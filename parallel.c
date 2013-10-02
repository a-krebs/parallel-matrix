/*
 * Parallel implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "shared.h"
#include "thread.h"

pthread_barrier_t barrier;
struct timeval phase2Start;
struct timeval phase2End;

int main(int argc, char *argv[]) {
	/* matrices */
	int **A = NULL;
	int **B = NULL;
	int **C = NULL;
	/* arguments */
	int argsParsed = 0;
	struct arguments *args = NULL;
	unsigned int seed = 0;
	int size = 0;
	/* threads */
	pthread_t *threads = NULL;
	struct threadArgs *threadArgs = NULL;
	/* Master thread has own argument struct outside of array.
	 * Making it part of the array was causing off-by-one when dispatching
	 * threads. */
	struct threadArgs *masterArgs = NULL;
	int numThreads = 0;
	int rStart = 0;
	int rEnd = 0;
	/* misc */
	int i = 0;
	/* timekeeping */
	struct timeval baseline;		// start
	struct timeval phase1End;		// before barrier
	struct timeval end;			// after phase 2
	
	/* get arguments */
	args = calloc(1, sizeof(struct arguments));
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	size = args->size;
	numThreads = args->procs -1;	/* -1 to account for master */
	seed = args->seed;

	/* PHASE 1 START */

	/* baseline time */
	gettimeofday(&baseline, NULL);

	/* set up matrices */
	if (setupExp(&A, &B, &C, size, seed) != 0) {
		exit(EXIT_FAILURE);
	}

	/* allocate memory for threads and arguments */
	threads = calloc(numThreads, sizeof(pthread_t));
	threadArgs = calloc(numThreads, sizeof(struct threadArgs));
	masterArgs = calloc(1, sizeof(struct threadArgs));

	/* initialize barrier */
	if (pthread_barrier_init(&barrier, NULL, args->procs) != 0) {
		printf("Initializing barrier failed.\n");
		exit(EXIT_FAILURE);
	}

	/* Split work between threads. */
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
		threadArgs[i].id = i;
	}
	/* set up master thread arguments */
	rEnd = getRangeEnd(rStart, args->procs, size);
	masterArgs->rStart = rStart;
	masterArgs->rEnd = rEnd;
	masterArgs->A = A;
	masterArgs->B = B;
	masterArgs->C = C;
	masterArgs->size = size;
	masterArgs->id = 0;

	/* Spin up threads */
	for (i = 0; i < numThreads; i++) {
		pthread_create(&threads[i], NULL, slave, &threadArgs[i]);
	}

	/* phase 1 time */
	gettimeofday(&phase1End, NULL);

	/* END PHASE 1 */
	/* BEGIN PHASE 2 */

	/* master thread work */
	slave((void *)masterArgs);

	/* PHASE 2 END */

#if VERIFY
	printMatrix(A, size);
	printMatrix(B, size);
#endif

	/* join threads */
	for (i = 0; i < numThreads; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Pthread_join fails");
		}
	}

	/* end time */
	gettimeofday(&end, NULL);

#if VERIFY
	printMatrix(C, size);
#else
	/* print times */
	
	printf("parallel,%d,%d,%d,", seed, size, args->procs);
	printElapsedTime(&baseline, &phase1End);
	printf(",");
	printElapsedTime(&phase2Start, &phase2End);
	printf(",");
	printElapsedTime(&baseline, &end);
	printf("\n");
#endif

	/* clean up and exit */
	tearDownExp(&A, &B, &C, size);
	free(args);
	free(threads);
	free(threadArgs);
	exit(EXIT_SUCCESS);
}
