#ifndef __UDP_H__
#define	__UDP_H__

#include "network.h"

#define	MSG_LENGTH	5
#define	UDP_PORT	"12345"

int		initUDPServer();
void*	processUDPServer(void*);

#endif
