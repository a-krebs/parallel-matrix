/*
 * Arguments given to each thread.
 */
struct threadArgs {
	int id;
	int rStart;
	int rEnd;
	int **A;
	int **B;
	int **C;
	int size;
};

extern pthread_barrier_t barrier;

/*
 * Global time structs to allow master thread to store phase 2 start
 * and end times.
 */
extern struct timeval phase2Start;
extern struct timeval phase2End;

/*
 * Thread work function.
 *
 * Each thread (including the master thread) performs this work.
 */
void *slave(void *args);

/*
 * If rStart is the start of the matrix column range (inclusive)
 * return the desired end of the range given matrix size s split
 * between p processors evenly.
 */
int getRangeEnd(int rStart, int p, int s);
