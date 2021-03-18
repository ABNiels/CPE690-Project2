#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int queue = 0;
char nextSideBuff[255];

int sleepTime;
char filepath[255];

int visitorsInHouse = 0, currentSide = 0;
pthread_mutex_t lock;

typedef struct _visitor {
	char side;
	int turn;
	pthread_t tid;
	struct _visitor *np;
} visitor;

visitor *head = NULL;

char *directions[] = {"left to right", "right to left"};

void *sendVisitor(void *guest) {
	printf("Visitor %i: Cross house request granted (Current crossing: %s, Number of visitors on house: %i)\n", ((visitor*)guest)->turn, directions[currentSide], visitorsInHouse);	
	sleep(sleepTime);
	pthread_mutex_lock(&lock);
	visitorsInHouse--;
	pthread_mutex_unlock(&lock);
	printf("Visitor %i: Exit house (Current crossing: %s, Number of visitors on house: %i)\n", ((visitor*)guest)->turn, directions[currentSide], visitorsInHouse);	
	return NULL;
}

void *getRequest(void *args) {
	
	FILE *fp = fopen(filepath, "r");
	
	/* Randomize when a new visitor shows up */
	time_t t;
	srand((unsigned) time(&t));

 	head = (visitor *) malloc(sizeof(visitor));
	visitor *ptr = head;
	int i = 0;
	while(fgets(nextSideBuff, 3, (FILE*)fp)) {
	
		/* Create singlely linked list for queue */
		ptr->side = (nextSideBuff[0]=='L')? 0:1;
		ptr->turn = i++;
		ptr->np = (visitor *)malloc(sizeof(visitor));
		
		/* Print arrival */
		printf("Visitor %i: Request to cross house (%s)\n", ptr->turn, directions[ptr->side]);
		if(visitorsInHouse >= 3 || ptr->side != currentSide){
			printf("Visitor %i: waiting\n", ptr->turn);
		}
		
		ptr = ptr->np;
		
		pthread_mutex_lock(&lock);
		queue++;
		pthread_mutex_unlock(&lock);
		usleep((100+rand()%200)*1000); // random delay between visitors
		while(queue>3);
		
	}
	ptr->np = NULL;	
	ptr = head;
	
		
}

void *handleRequest(void *args) {
	while(queue==0);
	visitor *ptr = head;
	while(queue != 0 && ptr!=NULL) {
		usleep(300*1000); // short delay for readability
		
		/* People are walking your way with enough room */
		if(visitorsInHouse < 3 && ptr->side == currentSide) {
			pthread_create(&ptr->tid, NULL, sendVisitor, (void *)ptr);  
			ptr = ptr->np;
			
			/* A visitor has been handled and entered the house */
			pthread_mutex_lock(&lock);
			queue--;
			visitorsInHouse++;
			pthread_mutex_unlock(&lock);
			
		}
		/* No one in house and it's your turn */
		else if(visitorsInHouse==0 && ptr->side != currentSide) {
		
			/* Change Side */
			pthread_mutex_lock(&lock);
			currentSide ^= 1;
			pthread_mutex_unlock(&lock);
			
			pthread_create(&ptr->tid, NULL, sendVisitor, (void *)ptr);
			ptr=ptr->np;
			
			/* A visitor has been handled and entered the house */
			pthread_mutex_lock(&lock);
			queue--;
			visitorsInHouse++;
			pthread_mutex_unlock(&lock);
			
		}
		
	}
	
	/* Join all threads */
	ptr = head;
	while(ptr!=NULL) {
		pthread_join(ptr->tid, NULL);
		ptr = ptr->np;
	}
}

int main(char argc, char *argv[]) {
	if(argc < 3) {
		printf("Need all parameters:\n\t format: %s <input_file> <cross_time_int>\n", argv[0]);
		return 1;
	}
	
	sleepTime = atoi(argv[2]);
	strcpy(filepath, argv[1]);

	pthread_mutex_init(&lock, NULL);

	pthread_t tid[2];
	
	pthread_create(&tid[0], NULL, (void *)getRequest, NULL); 
	pthread_create(&tid[1], NULL, (void *)handleRequest, NULL); 

	for(int i=0; i<2; i++) {
		pthread_join(tid[i], NULL);
	}
	pthread_mutex_destroy(&lock);

	return 0;
}
