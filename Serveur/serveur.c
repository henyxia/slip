#include <stdio.h>
#include <pthread.h>
#include <libthrd.h>
#include "tcp.h"
#include "udp.h"

void heellllo(void* arg)
{
	printf("Helllloooo ! \n");
}

int main(int argc,char *argv[])
{
	//int ret;
	newThread(heellllo, NULL, 0);
/*
	int sUDP = initUDPServer();
	if(sUDP != SOCKET_ERROR)
	{
		//ret = newThread(processUDPServer, &sUDP, sizeof(sUDP));
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
	while(1);
	return 0;
}
