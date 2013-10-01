/*
 * Parallel implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include "shared.h"

/* shared memory */
int **A = NULL;
int **B = NULL;
int **C = NULL;
int Size = 0;

int main(int argc, char *argv[]) {

	int argsParsed = 0;
	struct arguments *args = calloc(1, sizeof(struct arguments));

	argsParsed = parse_args(argc, argv, args);
	if (argsParsed == 1) {
		exit(EXIT_FAILURE);
	}

	Size = args->size;

	/* allocate memory for matrices */
	A = allocMatrixInt(Size);
	B = allocMatrixInt(Size);
	C = allocMatrixInt(Size);

	/* Spin up threads */

	/* barrier */
	printTOD();

	freeMatrixInt(A, Size);
	freeMatrixInt(B, Size);
	freeMatrixInt(C, Size);
	free(args);
	exit(EXIT_SUCCESS);
}
