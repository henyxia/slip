#include "udp.h"

int main(int argc,char *argv[])
{
	int sUDP=initUDPServer();
	if(sUDP == SOCKET_ERROR);
	processUDPServer(sUDP);
	return 0;
}
