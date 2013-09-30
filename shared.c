/*
 * Shared portions of matrix multiplication implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "shared.h"

#if TEST
	#include "minunit.h"
	#define TEST_SIZE 1000
#endif

/*
 * Given matrices A, B and C, multiply the given rows of A with the
 * appropriate columns in B and write to the correlated space in C.
 *
 * The row arguments should be zero-indexed and inclusive.
 *
 * Matrices should be allocated in memory and initialized as appropriate.
 */
void multiply(int **A, int **B, int **C, int size, int rStart, int rEnd) {
	int rowA = 0;
	int colA = 0;
	int rowB = 0;
	int colB = 0;
	int rowC = 0;
	int colC = 0;
	int product = 0;

	for (rowC = 0; rowC < size; rowC++) {
		for (colC = 0; colC < size; colC++) {
			product = 0;
			for (colA = 0; colA < size; colA++) {
				for (rowB = 0; rowB < size; rowB++) {
					product +=
					    A[rowC][colA] * B[colC][rowB];
				}
			}
			C[rowC][colC] = product;
		}
	}
}

/*
 * Print the given matrix to stdout.
 */
void printMatrix(int **m, int size) {
	int row = 0;
	int col = 0;

	for (row = 0; row < size; row++) {
		for (col = 0; col < size; col++) {
			printf("%d\t", m[row][col]);
		}
		printf("\n");
	}
}

/*
 * Allocate and zero out memory area for square matrix of given size.
 *
 * Returns pointer to matrix of integers or NULL on failure.
 */
int **allocMatrixInt(int size) {
	int i = 0;
	int j = 0;
	int **m = NULL;

	// initialize row pointers
	m = calloc(size, sizeof(int*));
	if (m == NULL) return NULL;

	for (i = 0; i < size; i++) {
		// allocate column pointers
		m[i] = calloc(size, sizeof(int));
		if (m[i] == NULL) return NULL;
		for (j = 0; j < size; j++) {
			// initialize column
			m[i][j] = 0;
		}
	}
	return m;
}

/*
 * Free the allocated matrix.
 */
void freeMatrixInt(int **m, int size) {
	int i = 0;

	for (i = 0; i < size; i++) {
		free(m[i]);
	}
	free(m);
}

/*
 * Fill matrix with random integers using random() with given seed.
 */
void initMatrixInt(int **m, int size, unsigned int seed){
	int i = 0;
	int j = 0;

	srandom(seed);

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			m[i][j] = (int)random();
		}
	}
}

/*
 * Parse arguments into given struct.
 * Struct memory area should be initialized.
 *
 * Code taken from
 * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 */
int parse_args(int argc, char *argv[], struct arguments* args) {
	/* expected arguments */
	int procs = 0;
	int size = 0;

	/* vars needed for parsing */
	int index;
	int option;

	opterr = 0;
	while ((option = getopt(argc, argv, "p:s:")) != -1) {
		switch (option) {
		case 'p':
			procs = atoi(optarg);
			break;
		case 's':
			size = atoi(optarg);
			break;
		case '?':
			if ((optopt == 'p') || (optopt == 's')) {
				fprintf(
				    stderr,
				    "Option -%s requires an argument.\n",
				    optopt);
			} else if (isprint(optopt)) {
				fprintf(
				    stderr,
				    "Unknown option `-%c'.\n",
				    optopt);
			} else {
				fprintf(
				    stderr,
				    "Unknown option character `\\x%x'.\n",
				    optopt);
			}
			/* fall through to default */
		default:
			return -1;
		}
	}
	
	printf("p = %d, s = %d\n", procs, size);
	for (index = optind; index < argc; index++) {
		printf ("Non-option argument %s\n", argv[index]);
	}

	args->procs = procs;
	args->size = size;
	return 0;
}

#if TEST

static char *test_multiply() {
	int testSize = 4;
	unsigned int testSeed = 1234;

	int **a = allocMatrixInt(testSize);
	int **b = allocMatrixInt(testSize);
	int **c = allocMatrixInt(testSize);

	initMatrixInt(a, testSize, testSeed);
	initMatrixInt(b, testSize, testSeed);

	multiply(a, b, c, testSize, 0, 0);
	printMatrix(a, testSize);
	printMatrix(b, testSize);
	printMatrix(c, testSize);
	return NULL;
}

static char *test_allocMatrixInt() {
	int testSize = TEST_SIZE;
	int i = 0;
	int j = 0;
	int **m = NULL;

	m = allocMatrixInt(testSize);
	mu_assert("Matrix memory not allocated.", m != NULL);

	for (i = 0; i < testSize; i++) {
		mu_assert("Matrix memory not allocated.", m[i] != NULL);
		for (j = 0; j < testSize; j++) {
			mu_assert("Matrix not initialized to 0.", m[i][j] == 0);
		}
	}
	freeMatrixInt(m, testSize);
	return NULL;
}

static char *test_initMatrixInt() {
	int testSize = TEST_SIZE;
	unsigned int testSeed = 1234;
	int i = 0;
	int j = 0;

	int **a = allocMatrixInt(testSize);
	int **b = allocMatrixInt(testSize);

	initMatrixInt(a, testSize, testSeed);
	initMatrixInt(b, testSize, testSeed);

	for (i = 0; i < testSize; i++) {
		for (j = 0; j < testSize; j++) {
			mu_assert("Matrix init a != b", a[i][j] == b[i][j]);
		}
	}
	freeMatrixInt(a, testSize);
	freeMatrixInt(b, testSize);
	return NULL;
}

char *test_all_shared() {
	mu_run_test(test_allocMatrixInt);
	mu_run_test(test_initMatrixInt);
	mu_run_test(test_multiply);
	return NULL;
}

#endif
