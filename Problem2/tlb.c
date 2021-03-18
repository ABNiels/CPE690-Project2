#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>

#include <unistd.h>

int main(char argc, char *argv[]) {
		
	int numpages = atoi(argv[1]), numtrials = atoi(argv[2]);
	int jump = getpagesize() / sizeof(int);
	
	char *a;
	a = malloc(numpages*jump);
	
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int k = 0; k < numtrials; k++) {
		for(int i =0; i < numpages * jump; i+=jump) {
			a[i]+=1;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%lf ", (double)(end.tv_nsec-start.tv_nsec)/numtrials/numpages);
}
