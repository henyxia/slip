#ifndef	__TCP_H__
#define	__TCP_H__

#include "network.h"

#define	TCP_PORT	"80"

int		initTCPServer();
void*	processTCPServer(void*);

#endif
