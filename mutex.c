#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TOT_THREADS       10
#define EACH_THREAD_TIMES 20

unsigned int globalMemo = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void * threadFunc(void *threadData)
{
	unsigned int i = 0;
	unsigned int data = 0;

	data = *((unsigned int *)threadData);

	for(i = 0; i < EACH_THREAD_TIMES; i++){
		pthread_mutex_lock(&mtx);

		globalMemo++;
		printf("Thread data: [%u]. globalMemo: [%u]\n", data, globalMemo);

		pthread_mutex_unlock(&mtx);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int ptRet = 0;
	unsigned int i = 0;
	pthread_t threads[TOT_THREADS];
	pthread_mutexattr_t mtx_attr;
	pthread_attr_t pt_attr;

	globalMemo = 0;

	/* Pthread attributes (none defined, like pthread_attr_setdetachstate(3)) */
	pthread_attr_init(&pt_attr);

	/* Mutter attributes */
	pthread_mutexattr_init(&mtx_attr);
	pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_FAST_NP);

	/* Launching threads */
	for(i = 0; i < TOT_THREADS; i++){
		ptRet = pthread_create(&threads[i], &pt_attr, threadFunc, (void *)&i);

		if(ptRet != 0){
			printf("ERRO pthread_create() [%d - %s]\n", ptRet, strerror(ptRet));
			return(-1);
		}
	}

	/* Waiting for all threads to does its work (pthread_detach(3)) */
	for(i = 0; i < TOT_THREADS; i++){
		printf("Join (waiting) for thread: [%d]\n", i);
		ptRet = pthread_join(threads[i], NULL); /* NULL, but I able to catch the value released by pthread_exit(3) */
	}

	/* Deleting mutex and mutex attributes */
	pthread_mutexattr_destroy(&mtx_attr);
	pthread_mutex_destroy(&mtx);

	/* Deleting pthread attributes */
	pthread_attr_destroy(&pt_attr);

	printf("Total: [%d]\n", globalMemo);

	return(0);
}
