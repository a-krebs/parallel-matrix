/*
 * Input arguments.
 */
struct arguments {
	int procs;
	int size;
	int seed;
};

/*
 * Given matrices A, B and C, multiply the given rows of A with the
 * appropriate columns in B and write to the correlated space in C.
 *
 * The row arguments should be zero-indexed and inclusive.
 *
 * Matrices should be allocated in memory and initialized as appropriate.
 */
void multiply(int **A, int **B, int **C, int size, int rStart, int rEnd);

/*
 * Print the time of day to stdout.
 */
void printTOD();

/*
 * Print the time elapsed since the given reference time.
 */
void printElapsedTime(struct timeval reference);

/* Subtract the `struct timeval' values X and Y (X - Y),
 * storing the result in RESULT.
 *
 * Return 1 if the difference is negative, otherwise 0.
 *
 * This function is taken from:
 * http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */
int timevalSubtract(struct timeval *result, struct timeval *x, struct timeval *y);

/*
 * Print the given matrix to stdout.
 */
void printMatrix(int **m, int size);

/*
 * Parse arguments into given struct.
 * Struct memory area should be initialized.
 */
int parse_args(int argc, char *argv[], struct arguments* args);

/*
 * Fill matrix with random integers using random().
 *
 * Run srandom() to seed random prior to calling if desired.
 */
void initMatrixInt(int **m, int size);

/*
 * Allocate and zero out memory area for square matrix of given size.
 *
 * Returns pointer to matrix of integers.
 */
int **allocMatrixInt(int size);

/*
 * Free the allocated matrix.
 */
void freeMatrixInt(int **m, int size);

#if TEST
/*
 * Expose only test_all function.
 */
char *test_all_shared();
#endif
