/*
 * Sequential implementation of matrix multiplication.
 */

#include <stdlib.h>
#include <stdio.h>
#include "shared.h"

int main(int argc, char *argv[]) {
	int **m = NULL;
	int argsParsed = 0;
	struct arguments *args = calloc(1, sizeof(struct arguments));
	
	argsParsed = parse_args(argc, argv, args);
	if (argsParsed == 1) {
		exit(EXIT_FAILURE);
	}

	m = allocMatrixInt(args->size);
	freeMatrixInt(m, args->size);
	exit(EXIT_SUCCESS);
}
