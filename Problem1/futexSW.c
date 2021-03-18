#define _GNU_SOURCE
#define SINGLE_CORE

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#include <linux/futex.h>

#define FLAG1 2
#define FLAG2 3

#define PERMISSIONS 0666

int iter = 1000;

void *child(void *arg) {
	int *addr = (int *)arg;
	/* Context 2 */
	for (int i = 0; i < iter; i++) {
		sched_yield();
		while (syscall(SYS_futex, addr, FUTEX_WAIT, FLAG1, NULL, NULL, 42)) sched_yield();
		*addr = FLAG2;
		while (!syscall(SYS_futex, addr, FUTEX_WAKE, 1, NULL, NULL, 42)) sched_yield();
	}
	return NULL;
}

int main(void) {
	struct timespec start, end;
	
	#ifdef SINGLE_CORE
	/* Set to run on one processor */
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(0, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);
	#endif 
	
	/* Create shared memory */
	const int shm_id = shmget(IPC_PRIVATE, sizeof (int), PERMISSIONS);
	int *addr = shmat(shm_id, NULL, 0);
	
	/* Create child thread to bounce between */
	pthread_t tid;
	pthread_create(&tid, NULL, child, addr);

	*addr = FLAG1;

	/* Time full runtime */
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	/* Context 1 */
	for (int i = 0; i < iter; i++) {
		*addr = FLAG1;
		while (!syscall(SYS_futex, addr, FUTEX_WAKE, 1, NULL, NULL, 42)) sched_yield();
		sched_yield();
		while (syscall(SYS_futex, addr, FUTEX_WAIT, FLAG2, NULL, NULL, 42)) sched_yield();
	}
	
	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%ld ns/switch\n", (end.tv_nsec - start.tv_nsec)/iter/4);

	return 0;
}
