struct threadArgs {
	int rStart;
	int rEnd;
	int **A;
	int **B;
	int **C;
	int size;
};

void *slave(void *args);
int getRangeEnd(int rStart, int p, int s);
