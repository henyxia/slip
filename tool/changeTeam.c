#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libcom.h>

#define MAX_TEAM	10
#define	CMD_SIZE	5

int main(void)
{
	unsigned char data[CMD_SIZE] = "azert";
	int i;
	int teamOld;
	int teamNew;
	printf("Do you want to change the team number ?\n");
	printf("Enter the current ID : \n");
	while(scanf("%d", &teamOld) != 1);
	printf("Enter the new ID : \n");
	while(scanf("%d", &teamNew) != 1);
	
	if(teamNew < 0 || teamNew > MAX_TEAM || teamOld < 0 || teamOld > MAX_TEAM)
	{
		printf("This team does not exist (%d -> %d)\n", teamOld, teamNew);
		return 0;
	}
	
	printf("Changing team %d to %d\n", teamOld, teamNew);

	data[0] = teamOld;
	data[1] = teamNew;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;

	for(i=0; i<10; i++)
	{
		envoiMessageUnicast(54321, 0xAC1A4FC9, data, sizeof(data));
		usleep(10000);
	}

	return 0;
}
