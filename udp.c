#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "udp.h"
#include "teams.h"

int processPacket(unsigned char * message)
{
	printf("Message recieve from team : %d aka %s\n", message[0] >> 4, teams[message[0] >> 4]);
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

