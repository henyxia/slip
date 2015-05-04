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
#include <unistd.h>
#include <netdb.h>

#include <stdbool.h>

#include "libcom.h"
#include "../Serveur/udp.h"

#define MAX_TAMPON	1024
//#define	MSG_LENGTH	MAX_TAMPON

bool stop = false;
int udpSock = SOCKET_ERROR;

// Common function
void shutdownServers(void)
{
	stop = true;
}

// UDP Functions
int initUDPServer(short int port)
{
	int s, status;
	unsigned sinlen;
	struct sockaddr_in sock_in;

	sinlen = sizeof(struct sockaddr_in);
	memset(&sock_in, 0, sinlen);

	s = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_in.sin_port = htons(port);
	sock_in.sin_family = PF_INET;

	int yes = 1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))<0){
		perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
		exit(-1);
	}

	if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes)<0)
	{
		perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
		exit(-1);
	}

	status = bind(s, (struct sockaddr *)&sock_in, sinlen);
	printf("Bind Status = %d\n", status);

	status = getsockname(s, (struct sockaddr *)&sock_in, &sinlen);
	printf("UDP Server listening on port %d\n",htons(sock_in.sin_port));

	udpSock = s;

	printf("UDP Server socket is %d\n", udpSock);

	return udpSock;

}

void serveurMessages(void (*func)(unsigned char *, int))
{
	printf("UDP Server succesfully started and now listening\n");
	int sServ = udpSock;
	printf("UDP Socket Internal %d\n", sServ);
	while(!stop)
	{
#ifdef DEBUG
		printf("Waiting for events\n");
#endif
		struct sockaddr_storage adresse;
		socklen_t taille=sizeof(adresse);
		unsigned char message[MSG_LENGTH];
		int nboctets=recvfrom(sServ,message,MSG_LENGTH,0,(struct sockaddr *)&adresse,&taille);
#ifdef DEBUG
		printf("New message recieved (%d length)\n", nboctets);
#endif
		message[nboctets]='\0';
#ifdef DEBUG
		printf("Message SND %02X%02X%02X%02X%02X\n", message[4], message[3], message[2], message[1], message[0]);
#endif
		if(nboctets==MSG_LENGTH)
			func(message, 0);
			//newUDPClient(message, 0);
	}
}

int envoiMessage(int port, unsigned char* str, int size)
{
#ifdef DEBUG
	printf("Initializing sending socket\n");
#endif
	struct sockaddr_in sin = { 0 };
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct hostent *hostinfo;

	if(sock == SOCKET_ERROR)
	{
		printf("Error with socket\n");
		return 1;
	}

	hostinfo = gethostbyname("172.26.79.255");
	if (hostinfo == NULL)
	{
		printf("Unable to resolve this address\n");
		return 2;
	}

	sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	int yep = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yep, sizeof(yep))<0)
	{
		printf("Unable to put broadcast flag\n");
		return 3;
	}

	if(sendto(sock, str, size, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0)
	{
		printf("Fail to send the message\n");
		return 4;
	}
#ifdef DEBUG
	else
		printf("Data send successfully\n");
#endif
	close(sock);

	return 0;
}

int envoiMessageUnicast(int target, int destIp, unsigned char* str, int size)
{
	return 0;
}

// TCP Functions
int initialisationServeur(char* service)
{
	struct addrinfo precisions,*resultat;
	int statut;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_STREAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&resultat);
	if(statut<0)
	{
		perror("initialisationServeur.getaddrinfo");
		exit(EXIT_FAILURE);
	}

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0)
	{
		perror("initialisationServeur.socket");
		exit(EXIT_FAILURE);
	}

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0)
	{
		perror("initialisationServeur.setsockopt (REUSEADDR)");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0)
	{
		perror("initialisationServeur.setsockopt (NODELAY)");
		exit(EXIT_FAILURE);
	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0)
		return -1;

	/* Liberation de la structure d'informations */
	freeaddrinfo(resultat);

	/* Taille de la queue d'attente */
	statut=listen(s, MAX_HTTP_CLIENTS);
	if(statut<0)
		return -1;

	return s;
}

void boucleServeur(int sServ, void(*func)(int))
{
	int newClient;
	while(!stop)
	{
    	if((newClient=accept(sServ,NULL,NULL))>=0)
			func(newClient);
    }

/*
		printf("Waiting for events\n");
		struct sockaddr_storage adresse;
		socklen_t taille=sizeof(adresse);
		unsigned char message[MSG_LENGTH];
		int nboctets=recvfrom(sServ,message,MSG_LENGTH,0,(struct sockaddr *)&adresse,&taille);
		message[nboctets]='\0';
		if(nboctets==MSG_LENGTH);
			//func(message);*/
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
