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

void	initTeams(void);
char*	getTeamMembers(int);
void	setTeamValues(int, unsigned char, unsigned char, unsigned char, unsigned char);

#endif
