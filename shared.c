/*
 * Shared portions of matrix multiplication implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "shared.h"

#if TEST
	#include "minunit.h"
	#define TEST_SIZE 1000
#endif

/*
 * Print the time of day to stdout.
 */
void printTOD() {
	char buffer[32]; // length is arbitrary
	struct timeval time;
	memset(&time, 0, sizeof(struct timeval));

	gettimeofday(&time, NULL);

	strftime(buffer, 32,"%H:%M:%S", localtime(&time.tv_sec));
	printf("%s.%03ld\n", buffer, time.tv_usec/1000);
}

/*
 * Print the time elapsed since r1 given r2
 * (IE r2 is the later time, r1 is the earlier time.
 */
void printElapsedTime(struct timeval *r1, struct timeval *r2) {
	struct timeval diff;

	if (timevalSubtract(&diff, r2, r1) != 0) {
		printf("Time difference negative!");
		return;
	}

	printf("%ld.%06ld", diff.tv_sec, diff.tv_usec);
}

/* Subtract the `struct timeval' values X and Y (X - Y),
 * storing the result in RESULT.
 *
 * Return 1 if the difference is negative, otherwise 0.
 *
 * This function is taken from:
 * http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */
int timevalSubtract(struct timeval *result, struct timeval *x, struct timeval *y) {
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	   tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

/*
 * Given matrices A, B and C, multiply the given rows of A with the
 * appropriate columns in B and write to the correlated space in C.
 *
 * The row arguments should be zero-indexed and inclusive.
 *
 * Matrices should be allocated in memory and initialized as appropriate.
 */
void multiply(int **A, int **B, int **C, int size, int rStart, int rEnd) {
	int rowC = 0;
	int colC = 0;
	int k = 0;
	int product = 0;

#if DEBUG
	printf("Multiplying and storing to: %p\n", (void *)C);
#endif

	for (rowC = rStart; rowC <= rEnd; rowC++) {
		for (colC = 0; colC < size; colC++) {
			product = 0;
			for (k = 0; k < size; k++) {
				product +=
				    A[rowC][k] * B[k][colC];
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

#if DEBUG
	printf("Printing from %p\n", (void*) m);
#endif

	for (row = 0; row < size; row++) {
		for (col = 0; col < size; col++) {
			printf("%d\t", m[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

/*
 * Allocate matrices A, B, and C to given size.
 * Initialize A and B with random data using given seed value.
 *
 * This is a wrapper function for calls to allocMatrixInt and initMatrixInt.
 *
 * Returns 0 if everything works, non-zero otherwise.
 */
int setupExp(int ***A, int ***B, int ***C, int size, unsigned int seed) {
	/* allocated memory */
	*A = allocMatrixInt(size);
	*B = allocMatrixInt(size);
	*C = allocMatrixInt(size);

	if ((*A == NULL) || (*B == NULL) || (*C == NULL)) {
		return -1;
	}

	/* initialize matric contents */
	srandom(seed);
	initMatrixInt(*A, size);
	initMatrixInt(*B, size);

	return 0;
}

/*
 * Clean up allocated matrix memory
 */
void tearDownExp(int ***A, int ***B, int ***C, int size) {
	freeMatrixInt(*A, size);
	freeMatrixInt(*B, size);
	freeMatrixInt(*C, size);
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
 * Fill matrix with random integers using random().
 *
 * Run srandom() to seed random prior to calling if desired.
 */
void initMatrixInt(int **m, int size){
	int i = 0;
	int j = 0;

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
	int procs = 1;		// default to 1
	int size = 0;
	unsigned int seed = 1000;

	/* vars needed for parsing */
	int index;
	int option;

	opterr = 0;
	while ((option = getopt(argc, argv, "p:s:k:")) != -1) {
		switch (option) {
		case 'p':
			procs = atoi(optarg);
			break;
		case 's':
			size = atoi(optarg);
			break;
		case 'k':
			option = atoi(optarg);
			break;
		case '?':
			if ((optopt == 'p')
			    || (optopt == 's')
			    || (optopt == 'k')){
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
	
#if DEBUG
	printf("p = %d, s = %d, k = %d\n", procs, size, seed);
#endif

	/* Make sure p is at least 1 */
	if (procs < 1) {
		printf("p must be at least 1.\n");
		return -1;
	}

	/* Make sure s is evenly divisible into p */
	if ((size > 0) && (size % procs != 0)) {
		printf("s must be evenly divisible into p\n");
		return -1;
	}
	for (index = optind; index < argc; index++) {
		printf("Non-option argument %s\n", argv[index]);
		return -1;
	}

	args->procs = procs;
	args->size = size;
	args->seed = seed;
	return 0;
}

#if TEST

static char *test_multiply() {
	int testSize = 3;
	int testValues[3] = {1,2,3};
	int testResults[3] = {6,12,18};
	int i = 0;
	int j = 0;
	unsigned int testSeed = 1234;

	int **a = allocMatrixInt(testSize);
	int **b = allocMatrixInt(testSize);
	int **c = allocMatrixInt(testSize);

	// initialize with known values
	for (i = 0; i < testSize; i++) {
		for (j = 0; j < testSize; j++) {
			a[i][j] = testValues[j];
			b[i][j] = testValues[j];
		}
	}
	
	// mutiply matrix in parts
	multiply(a, b, c, testSize, 0, 1);
	multiply(a, b, c, testSize, 2, 2);

	// test for known results
	for (i = 0; i < testSize; i++) {
		for (j = 0; j < testSize; j++) {
			mu_assert(
			    "Matrix multiplication incorrect.",
			    c[i][j] == testResults[j]);
		}
	}
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

	srandom(testSeed);
	initMatrixInt(a, testSize);
	initMatrixInt(b, testSize);

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
