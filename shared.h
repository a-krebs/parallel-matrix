/*
 * Input arguments.
 */
struct arguments {
	int procs;
	int size;
};

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

/*
 * Expose only test_all function.
 */
char *test_all_shared();
