#include <stdio.h>
#include <pthread.h>
#include <libthrd.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <libcom.h>
#include "http.h"
//#include "teams.h"
#include "udp.h"

struct sigaction	action;
bool				stop;

void hand(int sig)
{
	printf("SIGINT caught stopping TCP and UDP servers\n");
	shutdownServers();
	stop = true;
}
/*
void initTeams(void)
{
	int i;
	for(i=0; i<MAX_TEAMS; i++)
	{
		myTeams[i].x = 0;
		myTeams[i].y = 0;
		myTeams[i].z = 0;
		myTeams[i].t = 0;
	}
}
*/
void startTCPServer(void* arg)
{

	int *sTCP;
	sTCP = arg;
	boucleServeur(*sTCP, newHTTPClient);
}

void startUDPServer(void* arg)
{
	serveurMessages(12345, newUDPClient);
}

int main(int argc,char *argv[])
{
	// Yep, we want to run the soft
	stop = false;

	// SIGINT catch
	action.sa_handler = hand;
	sigaction(SIGINT, &action, NULL);

	// UDP
	if(initUDPServer(12345) != SOCKET_ERROR)
		newThread(startUDPServer, NULL, 0);
	else
		printf("UDP Init failed\n");
/*
	// TCP
	int sTCP = initialisationServeur("80");
	if(sTCP != SOCKET_ERROR)
		newThread(startTCPServer, &sTCP, sizeof(sTCP));
*/
	// Infinite wait
	while(!stop)
		sleep(1);

	// Wait for remaining thread before closing
	waitForThreads();

	printf("All thread stopped !\nGood Bye !\n");

	return 0;
}
