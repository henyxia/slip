#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "libcom.h"

#define MAX_TAMPON	1024
#define	MSG_LENGTH	MAX_TAMPON

#ifdef DEBUG
//static void afficheAdresse(FILE *flux,void *ip,int type);
//static void afficheAdresseSocket(FILE *flux,struct sockaddr_storage *padresse);
//static void afficheHote(FILE *flux,struct hostent *hote,int type);
#endif

// UDP Functions
void serveurMessages(short int port, void (*func)(unsigned char *, int))
{
	char service[8];
	sprintf(service, "%d", port);
	struct addrinfo precisions,*resultat;
	int statut;
	int s;

	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&resultat);

	if(statut<0)
	{
		perror("initialisationSocketUDP.getaddrinfo");
		exit(EXIT_FAILURE);
	}

	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0)
	{
		perror("initialisationSocketUDP.socket");
		exit(EXIT_FAILURE);
	}

	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0)
	{
		perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
		exit(-1);
	}

	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0)
	{
		perror("initialisationServeurUDP.bind");
		exit(-1);
	}

	freeaddrinfo(resultat);

	//TODO
	//Add the return into a global
	//return s;
}

int envoiMessage(int sock, unsigned char* str, int size)
{
	return 0;
}

int envoiMessageUnicast(int target, int destIp, unsigned char* str, int size)
{
	return 0;
}

// TCP Functions
int initialisationServeur(char* service)
{
	return 0;
}

void boucleServeur(int sServ, void(*func)(int))
{
	while(1)
	{
		printf("Waiting for events\n");
		struct sockaddr_storage adresse;
		socklen_t taille=sizeof(adresse);
		unsigned char message[MSG_LENGTH];
		int nboctets=recvfrom(sServ,message,MSG_LENGTH,0,(struct sockaddr *)&adresse,&taille);
		message[nboctets]='\0';
		if(nboctets==MSG_LENGTH);
			//func(message);
	}
}

// Debug functions
/*
static void afficheAdresse(FILE *flux,void *ip,int type)
{
	char adresse[MAX_TAMPON];
	inet_ntop(type,ip,adresse,MAX_TAMPON);
	fprintf(flux,adresse);
}
*/
/** Impression d'une adresse de socket **/
/*
static void afficheAdresseSocket(FILE *flux,struct sockaddr_storage *padresse)
{
	void *ip;
	int port;
	if(padresse->ss_family==AF_INET)
	{
	    struct sockaddr_in *ipv4=(struct sockaddr_in *)padresse;
	    ip=(void *)&ipv4->sin_addr;
    	port=ipv4->sin_port;
    }
	if(padresse->ss_family==AF_INET6)
	{
	    struct sockaddr_in6 *ipv6=(struct sockaddr_in6 *)padresse;
    	ip=(void *)&ipv6->sin6_addr;
	    port=ipv6->sin6_port;
    }
	fprintf(flux,"Adresse IP%s : ",padresse->ss_family==AF_INET?"v4":"v6");
	afficheAdresse(flux,ip,padresse->ss_family);
	fprintf(flux,"\nPort de la socket : %d.\n",ntohs(port));
}
*/
/** Impression des informations d'un hote **/
/*
static void afficheHote(FILE *flux,struct hostent *hote,int type)
{
	char **params;

	fprintf(flux,"Nom officiel : '%s'.\n",hote->h_name);
	fprintf(flux,"Surnoms: ");
	for(params=hote->h_aliases;*params!=NULL;params++)
	{
        fprintf(flux,"%s",*params);
        if(*(params+1)==NULL) fprintf(flux,",");
	}
	fprintf(flux,"\n");
	fprintf(flux,"Type des adresses   : %d.\n",hote->h_addrtype);
	fprintf(flux,"Taille des adresses : %d.\n",hote->h_length);
	fprintf(flux,"Adresses: ");
	for(params=hote->h_addr_list;params[0]!=NULL;params++)
	{
        afficheAdresse(flux,(struct in_addr *)params,type);
        if((*params+1)!=NULL) fprintf(flux,",");
	}
	fprintf(flux,"\n");
}
*/
