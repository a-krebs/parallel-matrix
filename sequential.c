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

	int argsParsed = 0;
	struct arguments *args = calloc(1, sizeof(struct arguments));
	
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed != 0) {
		exit(EXIT_FAILURE);
	}

	size = args->size;

	/* allocate memory space for matrices */
	A = allocMatrixInt(size);
	B = allocMatrixInt(size);
	C = allocMatrixInt(size);
	
	/* initialize matrix contents */
	initMatrixInt(A, size);
	initMatrixInt(B, size);

	printTOD();

	/* multiply */
	multiply(A, B, C, size, 0, size - 1);

	printTOD();

	freeMatrixInt(A, size);
	freeMatrixInt(B, size);
	freeMatrixInt(C, size);
	exit(EXIT_SUCCESS);
}
