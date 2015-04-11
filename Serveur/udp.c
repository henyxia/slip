#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "udp.h"
#include "teams.h"
#include "parity.h"

typedef int SOCKET;

void newUDPClient(unsigned char* message, int sock)
{
	int team = message[0] >> 4;
	if(team < 0 || team >= MAX_TEAMS)
	{
		printf("\tWrong team number");
		//return -1;
	}
	printf("Team %d (aka %s)\n", message[0] >> 4, getTeamMembers(message[0] >> 4));
	if(checkParity(message[1] + ((message[0] & 0x08) >> 3)))
	{
		printf(" \u21B3 X Parity error (X:%02x p:%02x t:%02x)\n", message[1], ((message[0] & 0x08) >> 3), (message[1] + ((message[0] & 0x08) >> 3)));
		//return -2;
	}
	if(checkParity(message[2] + ((message[0] & 0x04) >> 2)))
	{
		printf(" \u21B3 Y Parity error\n");
		//return -3;
	}
	if(checkParity(message[3] + ((message[0] & 0x02) >> 1)))
	{
		printf(" \u21B3 Z Parity error\n");
		//return -4;
	}
	if(checkParity(message[4] + (message[0] & 0x01)))
	{
		printf(" \u21B3 T Parity error\n");
		//return -5;
	}

	if(((message[0] & 0x0F) == 0x0F) && (message[1] == 0xFF) && (message[2] == 0xFF) && (message[3] == 0xFF) && (message[4] == 0xFF))
		printf(" \u21B3 Somebody died\n");
	else
	{
		printf(" \u21B3 X:%04d Y:%04d Z:%04d\n", message[1], message[2], message[3]);
		printf(" \u21B3 Temp: %02d\u2103\n", message[4]);
		setTeamValues(message[0] >> 4, message[1], message[2], message[3], message[4]);
	}
}
