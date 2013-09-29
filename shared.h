/*
 * Input arguments.
 */
struct arguments {
	int procs;
	int size;
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
 * Print the given matrix to stdout.
 */
void printMatrix(int **m, int size);

/*
 * Parse arguments into given struct.
 * Struct memory area should be initialized.
 */
int parse_args(int argc, char *argv[], struct arguments* args);

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
