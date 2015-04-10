#ifndef __LIB_COM__
#define __LIB_COM__

#define	SOCKET_ERROR	-1

void	serveurMessages(short int, void (*)(unsigned char *, int));
int		envoiMessage(int, unsigned char*, int);
int		envoiMessageUnicast(int, int, unsigned char*, int);
int		initialisationServeur(char*);
void	boucleServeur(int, void (*)(int));

#endif
