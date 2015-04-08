#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "libthrd.h"

#define MAX_THREAD	100

typedef struct
{
	void (*func) (void*);
	void* param;
}rFunc, *pFunc;

int threadStarted = 0;
pthread_t thread[MAX_THREAD];

void* processThread(void* arg)
{
	pFunc tArg = arg;
	tArg->func(tArg->param);
	free(tArg->param);
	free(tArg);
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
	//TODO
	//Add detached state
	if(pthread_create(&(thread[threadStarted]), NULL, processThread, newParam) != 0)
		return 3;

	return 0;
}
