#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <sys/syscall.h>

int main(char argc, char *argv[]) {
	struct timespec start, end;
	
	clock_gettime(CLOCK_MONOTONIC, &start);

	for(int i = 0; i<1000; i++) {
		syscall(SYS_gettid);
	}	
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("%lf ns\n", (double)(end.tv_nsec - start.tv_nsec)/1000);
	return 0;
}
