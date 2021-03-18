#include <stdio.h>
#include <time.h>

int main(char argc, char *argv[]) {
	struct timespec start, end, p1, p2, p3;
	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &p1);
	clock_gettime(CLOCK_MONOTONIC, &p2);
	clock_gettime(CLOCK_MONOTONIC, &p3);
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%ld, %ld, %ld, %ld\n", p1.tv_nsec - start.tv_nsec,
				       p2.tv_nsec - p1.tv_nsec,
				       p3.tv_nsec - p2.tv_nsec,
				       end.tv_nsec - p3.tv_nsec);
	return 0;
}
