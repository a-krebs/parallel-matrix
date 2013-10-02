/*
 * Sequential implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include "shared.h"

int main(int argc, char *argv[]) {
	/* matrices */
	int **A = NULL;
	int **B = NULL;
	int **C = NULL;
	/* arguments */
	int size = 0;
	unsigned int seed = 0;
	struct arguments *args = NULL;
	int argsParsed = 0;
	/* timekeeping */
	struct timeval baseline;	// start
	struct timeval start;		// after init, before work
	struct timeval end;		// after work

	/* get arguments */
	args = calloc(1, sizeof(struct arguments));
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	size = args->size;
	seed = args->seed;

	/* PHASE 1 START */

	/* baseline time */
	gettimeofday(&baseline, NULL);

	/* set up matrices */
	if (setupExp(&A, &B, &C, size, seed) != 0) {
		exit(EXIT_FAILURE);
	}
	/* PHASE 1 END */
	/* PHASE 2 START */

	/* post-setup time*/
	gettimeofday(&start, NULL);
	
#if VERIFY
	printMatrix(A, size);
	printMatrix(B, size);
#endif

	/* multiply */
	multiply(A, B, C, size, 0, size - 1);

	/* PHASE 2 END*/

	/* end time */
	gettimeofday(&end, NULL);

#if VERIFY
	printMatrix(C, size);
#endif

	/* print times */
#if !VERIFY
	printf("sequential,%d,%d,%d,", seed, size, args->procs);
	printElapsedTime(&baseline, &start);
	printf(",");
	printElapsedTime(&start, &end);
	printf(",");
	printElapsedTime(&baseline, &end);
	printf("\n");
#endif

	/* clean up and exit */
	tearDownExp(&A, &B, &C, size);
	free(args);
	exit(EXIT_SUCCESS);
}
