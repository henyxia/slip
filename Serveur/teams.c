#include <stdio.h>
#include "teams.h"

team myTeams[MAX_TEAMS];

char teams[MAX_TEAMS][25] = {"Jean & Flavien",
	"Cyril & JM",
	"Kevin & Benjamin",
	"Valentin & Alexander",
	"Timothee & Mageshwaran",
	"Jeremie & Julien",
	"Mehdi & Thibault",
	"Romain & Alexandre",
	"Sandra & Elise",
	"Hideo & Jerome",
	"Arnaud"};

void initTeams(void)
{
	int i;
	for(i=0; i<MAX_TEAMS; i++)
	{
		myTeams[i].x = 0;
		myTeams[i].y = 0;
		myTeams[i].z = 0;
		myTeams[i].t = 0;
		myTeams[i].lastUpdate = 0;
	}
}

char* getTeamMembers(int team)
{
	return teams[team];
}

void setTeamValues(int team, unsigned char x, unsigned char y, unsigned char z, unsigned char t, unsigned int lastUpdate)
{
	printf("New values set to X %d Y %d Z %d T %d LU %u\n", x, y, z, t, lastUpdate);
	myTeams[team].x = x;
	myTeams[team].y = y;
	myTeams[team].z = z;
	myTeams[team].t = t;
	myTeams[team].lastUpdate = lastUpdate;
}

int getTeamValue(int team, unsigned char query)
{
	if(query == 'X')
		return myTeams[team].x;
	else if(query == 'Y')
		return myTeams[team].y;
	else if(query == 'Z')
		return myTeams[team].z;
	else if(query == 'T')
		return myTeams[team].t;
	return -1;
}

team* getTeamValues(int team)
{
	return &(myTeams[team]);
}
