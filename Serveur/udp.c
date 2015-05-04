#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <libthrd.h>
#include <libcom.h>
#include "udp.h"
#include "teams.h"
#include "parity.h"

#define MAX_TEAM_FILE	32

typedef int SOCKET;

typedef struct
{
	unsigned char message[5];
	int sock;
}UDPMessage, *pUDPMessage;

void processUDPClient(void *arg)
{
	struct timeval tv;
	pUDPMessage myMessage = arg;
	unsigned char	message[6];
	//int sock = myMessage->sock;
#ifdef DEBUG
	printf("Message RCV %02X%02X%02X%02X%02X\n", myMessage->message[4], myMessage->message[3], myMessage->message[2], myMessage->message[1], myMessage->message[0]);
#endif

	//strncpy((char*)message, (char*)myMessage->message, 6);

	message[4] = myMessage->message[4];
	message[3] = myMessage->message[3];
	message[2] = myMessage->message[2];
	message[1] = myMessage->message[1];
	message[0] = myMessage->message[0];

	int team = message[0] >> 4;
	if(team < 0 || team >= MAX_TEAMS)
	{
#ifdef DEBUG
		printf(" \u21B3 Wrong team number\n");
#endif
		return;
	}
#ifdef DEBUG
	printf("Team %d (aka %s)\n", message[0] >> 4, getTeamMembers(message[0] >> 4));
#endif
	if(checkParity(message[1] + ((message[0] & 0x08) >> 3)))
	{
#ifdef DEBUG
		printf(" \u21B3 X Parity error (X:%02x p:%02x t:%02x)\n", message[1], ((message[0] & 0x08) >> 3), (message[1] + ((message[0] & 0x08) >> 3)));
#endif
		return;
	}
	if(checkParity(message[2] + ((message[0] & 0x04) >> 2)))
	{
#ifdef DEBUG
		printf(" \u21B3 Y Parity error\n");
#endif
		return;
	}
	if(checkParity(message[3] + ((message[0] & 0x02) >> 1)))
	{
#ifdef DEBUG
		printf(" \u21B3 Z Parity error\n");
#endif
		return;
	}
	if(checkParity(message[4] + (message[0] & 0x01)))
	{
#ifdef DEBUG
		printf(" \u21B3 T Parity error\n");
#endif
		return;
	}

	if(((message[0] & 0x0F) == 0x0F) && (message[1] == 0xFF) && (message[2] == 0xFF) && (message[3] == 0xFF) && (message[4] == 0xFF))
	{
		printf(" \u21B3 Somebody died in team %d\n", message[0] >> 4);
		unsigned char blinkDontDie[] = "azert";
		blinkDontDie[1] = 0;
		blinkDontDie[2] = 0;
		blinkDontDie[3] = 0;
		blinkDontDie[4] = 50;
		blinkDontDie[0] = (message[0] >> 4) + checkParity(blinkDontDie[4]);
		envoiMessage(12345, blinkDontDie, sizeof(blinkDontDie));
#ifdef DEBUG
		printf(" \u21B3 Blinking time ! (CMD %05X\n", blinkDontDie);
#endif
	}
	else
	{
		printf(" \u21B3 X:%04d Y:%04d Z:%04d\n", message[1], message[2], message[3]);
		printf(" \u21B3 Temp: %02d\u2103\n", message[4]);
		gettimeofday(&tv, NULL);
		printf(" \u21B3 Last update since epoch: %u\n", (unsigned int) tv.tv_sec);
		setTeamValues(message[0] >> 4, message[1], message[2], message[3], message[4], (unsigned int) tv.tv_sec);
		P(message[0] >> 4);
		FILE* teamFile = NULL;
		char teamFileName[MAX_TEAM_FILE];
		sprintf(teamFileName, "team%d.bin", message[0] >> 4);
		teamFile = fopen(teamFileName, "ab");
		if(teamFile == NULL)
		{
			printf("Unable to open the team %d log\n", message[0] >> 4);
			return;
		}
		fwrite(getTeamValues(message[0] >> 4), sizeof(team), 1, teamFile);
		fclose(teamFile);
		V(message[0] >> 4);
	}
}

void newUDPClient(unsigned char* message, int sock)
{
	pUDPMessage myMessage = NULL;

	myMessage = (pUDPMessage) malloc(sizeof(UDPMessage));
	myMessage->message[4] = message[4];
	myMessage->message[3] = message[3];
	myMessage->message[2] = message[2];
	myMessage->message[1] = message[1];
	myMessage->message[0] = message[0];
	//strncpy((char*)myMessage->message, (char*)message, 6);
	myMessage->sock = sock;

	if(myMessage == NULL)
	{
		printf("Unable to allocate a new UDP client\n");
		return;
	}

	newThread(processUDPClient, myMessage, sizeof(UDPMessage));
}

