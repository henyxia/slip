#include <stdio.h>
#include <pthread.h>
#include "tcp.h"
#include "udp.h"

int main(int argc,char *argv[])
{
	int ret;
	pthread_t tUDP;
	pthread_t tTCP;

	int sUDP = initUDPServer();
	if(sUDP != SOCKET_ERROR)
	{
		ret = pthread_create(&tUDP, NULL, processUDPServer, (void*) &sUDP);
		if(ret != 0)
		{
			printf("UDP Thread failed starting\n");
			return 2;
		}
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

	pthread_join(tUDP, NULL);

	return 0;
}
