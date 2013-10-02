struct threadArgs {
	int rStart;
	int rEnd;
	int **A;
	int **B;
	int **C;
	int size;
};

extern pthread_barrier_t barrier;

void *slave(void *args);
int getRangeEnd(int rStart, int p, int s);
