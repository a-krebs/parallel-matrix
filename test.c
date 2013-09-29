#include <stdio.h>
#include "minunit.h"
#include "test_shared.c"

int tests_run = 0;

int main(int argc, char *argv[]) {
	char *result = test_all_shared();
	if (result != NULL) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != NULL;
}
