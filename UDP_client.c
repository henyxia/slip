#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

void messageUDP(char *hote,char *service,unsigned char *message,int taille){

	struct addrinfo precisions,*resultat;
	int statut;
	int s;

	/* Creation de l'adresse de socket */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	statut=getaddrinfo(hote,service,&precisions,&resultat);
	if(statut<0){ perror("messageUDPgenerique.getaddrinfo"); exit(EXIT_FAILURE); }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("messageUDPgenerique.socket"); exit(EXIT_FAILURE); }

	/* Option sur la socket */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&vrai,sizeof(vrai))<0){
		perror("initialisationServeurUDPgenerique.setsockopt (BROADCAST)");
		exit(-1);
	}

	/* Envoi du message */
	int nboctets=sendto(s,message,taille,0,resultat->ai_addr,resultat->ai_addrlen);
	if(nboctets<0){ perror("messageUDPgenerique.sento"); exit(EXIT_FAILURE); }

	/* Liberation de la structure d'informations */
	freeaddrinfo(resultat);

	/* Fermeture de la socket d'envoi */
	close(s);
}

int main(int argc,char *argv[]){

	if(argc!=3){
		fprintf(stderr,"Syntaxe : %s <serveur> <message>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	char *hote=argv[1];
	char *message=argv[2];
	char *service="4000";
	messageUDP(hote,service,(unsigned char *)message,strlen(message));
	return 0;

}

