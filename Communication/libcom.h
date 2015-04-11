#ifndef __LIB_COM__
#define __LIB_COM__

#define	SOCKET_ERROR	-1

int		initUDPServer(short int);
void	serveurMessages(void (*)(unsigned char *, int));
int		envoiMessage(int, unsigned char*, int);
int		envoiMessageUnicast(int, int, unsigned char*, int);
int		initialisationServeur(char*);
void	boucleServeur(int, void (*)(int));
void	shutdownServers(void);

#endif
