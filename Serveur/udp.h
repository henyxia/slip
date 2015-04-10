#ifndef __UDP_H__
#define	__UDP_H__

#define	MSG_LENGTH	5
#define	UDP_PORT	"12345"

// Old
int		initUDPServer(short int);
void*	processUDPServer(void*);

// New
void	newUDPClient(unsigned char*, int);

#endif
