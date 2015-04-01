#include "parity.h"

bool checkParity(unsigned char data)
{
	unsigned int b=1;
	unsigned int count=0;
	unsigned int i;

	for(i=0; i<8; i++)
		if(data & (b << i))
			count++;

	return !(count % 2);
}
