#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <libcom.h>

int main(void)
{
	printf("Starting flooder\n");
	unsigned char data[] = "azert";
	srand(time(NULL));

	while(1)
	{
		data[0] = rand() % 255;
		data[1] = rand() % 255;
		data[2] = rand() % 255;
		data[3] = rand() % 255;
		data[4] = rand() % 255;
		envoiMessage(12345, data, sizeof(data));
		usleep(10000);
	}

	return 0;
}
