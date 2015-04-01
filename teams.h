#ifndef	__TEAMS_H__
#define	__TEAMS_H__

#define	MAX_TEAMS	11

typedef struct
{
	unsigned char x;
	unsigned char y;
	unsigned char z;
	unsigned char t;
}team;

const char teams[MAX_TEAMS][25] = {"Jean & Flavien",
	"Cyril",
	"Kevin",
	"Valentin & Alexander",
	"Timothee & Mageshwaran",
	"Jeremie & Julien",
	"Mehdi",
	"Romain & Alexandre",
	"Sandra & Elise",
	"Hideo",
	"Arnaud"};

team myTeams[MAX_TEAMS];

#endif
