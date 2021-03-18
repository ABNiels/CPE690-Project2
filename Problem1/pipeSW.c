#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>

int main() {

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);

	char buff1[10], buff2[10];
	int p1[2], p2[2], pid;

	if(pipe(p1) < 0 || pipe(p2) < 0) return -1;

	struct timespec start, end, t1, t2;
	clock_gettime(CLOCK_MONOTONIC, &start);
	if( (pid=fork()) > 0) {
		clock_gettime(CLOCK_MONOTONIC, &t1);
		for(int i = 0; i<1000; i++) {
			write(p1[1], buff2, 10);
			read(p2[0], buff1, 10);
		}
		clock_gettime(CLOCK_MONOTONIC, &t2);
	}
	else {
		clock_gettime(CLOCK_MONOTONIC, &t1);
		for(int i = 0; i<1000; i++) {
			write(p2[1], buff1, 10);
			read(p1[0], buff2, 10);
		}
		clock_gettime(CLOCK_MONOTONIC, &t2);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	long int runtime = end.tv_nsec-start.tv_nsec, threadtime = t2.tv_nsec-t1.tv_nsec;
	printf("%ld ns/switch\n", (runtime-threadtime)/2000);
	return 0;
}
