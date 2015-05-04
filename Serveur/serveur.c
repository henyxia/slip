#include <stdio.h>
#include <pthread.h>
#include <libthrd.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <libcom.h>
#include <getopt.h>
#include <string.h>
#include "http.h"
#include "teams.h"
#include "udp.h"

#define	MAX_PORT	5

struct sigaction	action;
bool				stop;

void hand(int sig)
{
	printf("SIGINT caught stopping TCP and UDP servers\n");
	shutdownServers();
	stop = true;
}

void startTCPServer(void* arg)
{

	int *sTCP;
	sTCP = arg;
	boucleServeur(*sTCP, newHTTPClient);
}

void startUDPServer(void* arg)
{
	serveurMessages(newUDPClient);
}

int main(int argc,char *argv[])
{
	// Option parser
	char			ret;
	const char*		short_opt = "hp:";
	char			port[MAX_PORT];
	strcpy(port, "80");
	struct option	long_opt[] =
	{
		{"help",	no_argument,		NULL,	'h'},
		{"port",	required_argument,	NULL,	'p'},
		{NULL,		0,					NULL,	0}
	};

	while((ret = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1)
	{
		switch(ret)
		{
			case -1:       /* no more arguments */
			case 0:        /* long options toggles */
				break;

			case 'p':
				sprintf(port, "%s", optarg);
				printf("Overriding default port (80) with port %s\n", port);
				break;

			case 'h':
				printf("Usage: %s [OPTIONS]\n", argv[0]);
				printf("\t-p, --port <PORT>\t\tOverride default port (80) with <PORT>\n");
				printf("\t-h, --help\t\tPrint this help and exit\n");
				printf("\n");
				return(0);

			case ':':
			case '?':
				fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
				return(-2);

			default:
				fprintf(stderr, "%s: invalid option -- %c\n", argv[0], ret);
				fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
				return(-2);
      };
   };

	// Yep, we want to run the soft
	stop = false;

	// Initializing teams & threads
	initTeams();
	initThreads(MAX_TEAMS);

	// SIGINT catch
	action.sa_handler = hand;
	sigaction(SIGINT, &action, NULL);
	
	// UDP
	if(initUDPServer(12345) != SOCKET_ERROR)
		newThread(startUDPServer, NULL, 0);
	else
		printf("UDP Init failed\n");

	// TCP
	int sTCP = initialisationServeur(port);
	if(sTCP != SOCKET_ERROR)
		newThread(startTCPServer, &sTCP, sizeof(sTCP));
	else
		printf("TCP Init failed\n");

	// Infinite wait
	while(!stop)
		sleep(1);

	// Wait for remaining thread before closing
	waitForThreads();

	printf("All thread stopped !\nGood Bye !\n");

	return 0;
}
