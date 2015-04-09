#include <stdio.h>
#include <pthread.h>
#include <libthrd.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include "tcp.h"
#include "udp.h"

struct sigaction	action;
bool				stop;

void heellllo(void* arg)
{
	printf("Helllloooo ! \n");
}

void hand(int sig)
{
	printf("SIGINT caught stopping TCP and UDP servers\n");
	stop = true;
}

int main(int argc,char *argv[])
{
	// Yep, we want to run the soft
	stop = false;

	// SIGINT catch
	action.sa_handler = hand;
	sigaction(SIGINT, &action, NULL);

	// Initialization UDP
	//int ret;
	/*
	int sUDP = initUDPServer();
	if(sUDP != SOCKET_ERROR)
	{
		ret = newThread(processUDPServer, &sUDP, sizeof(sUDP));
		if(ret != 0)
			return 32 + ret;
	}
	else
		return 1;

	int sTCP = initTCPServer();
	if(sTCP != SOCKET_ERROR)
	{
		ret = pthread_create(&tTCP, NULL, processTCPServer, (void*) &sTCP);
		if(ret != 0)
		{
			printf("TCP Thread failed starting\n");
			return 3;
		}
	}
	*/

	// Infinite wait
	while(!stop)
		sleep(1);

	// Wait for remaining thread before closing
	waitForThreads();

	printf("All thread stopped !\nGood Bye !\n");

	return 0;
}
