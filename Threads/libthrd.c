#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "libthrd.h"

#define MAX_THREAD	100
#define KEY			0x1100

typedef struct
{
	void (*func) (void*);
	void* param;
}rFunc, *pFunc;

int			threadStarted = 0;
pthread_t	thread[MAX_THREAD];
int			mySem;

void* processThread(void* arg)
{
	pFunc tArg = arg;
	tArg->func(tArg->param);
	free(tArg->param);
	free(tArg);
	threadStarted--;
	return NULL;
}

int newThread(void(*func) (void*), void* param, int size)
{
	pFunc newParam = (pFunc) malloc(sizeof(rFunc));
	if(newParam == NULL)
		return 1;
	newParam->func = func;
	newParam->param = (pFunc) malloc(size);
	if(newParam->param == NULL)
		return 2;
	if(size > 0)
		memcpy(newParam->param, param, size);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&(thread[threadStarted]), &attr, processThread, newParam) != 0)
		return 3;
	else
		threadStarted++;

	return 0;
}

void waitForThreads()
{
	printf("Waiting for thread to stop\n");
	while(threadStarted > 0)
	{
		printf("Still wait for %d thread\n", threadStarted);
		sleep(1);
	}
}

int get_sem(int i)
{
	ushort semarr[30];
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
	}arg;

	arg.array = semarr;
	semctl(mySem, i, GETALL, arg);
	return semarr[i];
}

void show_sem(int i)
{
	int val;
	val = get_sem(i);
	printf("semaphore[%d]=%d\n", i, val);
}

void create_sem(int N)
{
#ifdef DEBUG
	printf("Creating %d semaphores\n", N);
#endif
	mySem = semget(KEY, N, 0666 | IPC_CREAT);
	if(mySem < 0)
		printf("Unable to create the semaphore\n");
}

void init_sem(int N)
{
	int j;
	int retval;
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
	}arg;
	arg.val = 1;
	for (j=0; j<N; j++)
	{
		retval = semctl(mySem, j, SETVAL, arg);
		if(retval < 0)
			printf("Unable to initialize the semaphore\n");
	}
}

void initThreads(int sem)
{
	create_sem(sem);
	init_sem(sem);
}


void PV(int i, int act)
{
	struct sembuf op;
	int retval;

	op.sem_num = i;
	op.sem_op = act; //1=V, -1=P
	op.sem_flg = 0; //will wait
	retval = semop(mySem, &op, 1);
	if(retval != 0)
		printf("Error semop will do %d\n", act);
}

void P(int i)
{
	PV(i, -1);
}

void V(int i)
{
	PV(i, 1);
}
