#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "udp.h"
#include "teams.h"
#include "parity.h"

int processPacket(unsigned char * message)
{
	int team = message[0] >> 4;
	if(team < 0 || team >= MAX_TEAMS)
	{
		printf("\tWrong team number");
		return -1;
	}
	printf("Team %d (aka %s)\n", message[0] >> 4, teams[message[0] >> 4]);
	if(checkParity(message[1] + ((message[0] & 0x08) >> 3)))
	{
		printf(" \u21B3 X Parity error (X:%02x p:%02x t:%02x)\n", message[1], ((message[0] & 0x08) >> 3), (message[1] + ((message[0] & 0x08) >> 3)));
		return -2;
	}
	if(checkParity(message[2] + ((message[0] & 0x04) >> 2)))
	{
		printf(" \u21B3 Y Parity error\n");
		return -3;
	}
	if(checkParity(message[3] + ((message[0] & 0x02) >> 1)))
	{
		printf(" \u21B3 Z Parity error\n");
		return -4;
	}
	if(checkParity(message[4] + (message[0] & 0x01)))
	{
		printf(" \u21B3 T Parity error\n");
		return -5;
	}

	printf(" \u21B3 X:%04d Y:%04d Z:%04d\n", message[1], message[2], message[3]);
	printf(" \u21B3 Temp: %02d\u2103\n", message[4]);
	return 0;
}

int initUDPServer()
{
	char service[] = UDP_PORT;
	struct addrinfo precisions,*resultat;
	int statut;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&resultat);
	if(statut<0){ perror("initialisationSocketUDP.getaddrinfo"); exit(EXIT_FAILURE); }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationSocketUDP.socket"); exit(EXIT_FAILURE); }

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
		exit(-1);
	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) {perror("initialisationServeurUDP.bind"); exit(-1);}

	/* Liberation de la structure d'informations */
	freeaddrinfo(resultat);

	return s;

}

void* processUDPServer(SOCKET sServ)
{
	while(1)
	{
		printf("Waiting for events\n");
		struct sockaddr_storage adresse;
		socklen_t taille=sizeof(adresse);
		unsigned char message[MSG_LENGTH];
		int nboctets=recvfrom(sServ,message,MSG_LENGTH,0,(struct sockaddr *)&adresse,&taille);
		message[nboctets]='\0';
		if(nboctets==MSG_LENGTH)
			processPacket(message);
	}

	return NULL;
}

