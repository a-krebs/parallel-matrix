/*
 * Sequential implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include "shared.h"

int main(int argc, char *argv[]) {
	int **A = NULL;
	int **B = NULL;
	int **C = NULL;
	int size = 0;
	unsigned int seed = 0;

	int argsParsed = 0;
	struct arguments *args = calloc(1, sizeof(struct arguments));
	
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	size = args->size;
	seed = args->seed;

	/* allocate memory space for matrices */
	A = allocMatrixInt(size);
	B = allocMatrixInt(size);
	C = allocMatrixInt(size);
	
	/* initialize matrix contents */
	srandom(seed);
	initMatrixInt(A, size);
	initMatrixInt(B, size);
	
#if VERIFY
	printMatrix(A, size);
	printMatrix(B, size);
#else
	printTOD();
#endif

	/* multiply */
	multiply(A, B, C, size, 0, size - 1);

#if VERIFY
	printMatrix(C, size);
#else
	printTOD();
#endif

	freeMatrixInt(A, size);
	freeMatrixInt(B, size);
	freeMatrixInt(C, size);
	free(args);
	exit(EXIT_SUCCESS);
}
