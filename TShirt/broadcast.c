#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "serial.h"
#include "ethernet.h"
//#include "socket.h"
#include "serial.h"
#include "analog.h"
#include "parity.h"

#define MAC_SIZE	6
#define IPV4_SIZE	4

void datagrammeIP(uint8_t data [],int x, int y, int z, int t, int id)
{
	//Data
	data[0] = t;
	data[1] = z;
	data[2] = y;
	data[3] = x;
	data[4] = id;
	
	//UDP
	data[5] = 0x00;		//Checksum UDP
	data[6] = 0x00;		//which is not activated
	data[7] = 0x0D;
	data[8] = 0x00;
	data[9] = 0x39;
	data[10] = 0x30;
	data[11] = 0x10;
	data[12] = 0xA4;

	//IP
	data[13] = 0x00;
	data[14] = 0xF0;
	data[15] = 0xFF;
	data[16] = 0xFF;
	data[17] = 0xC8;
	data[18] = 0x4F;
	data[19] = 0x1A;
	data[20] = 0xAC;
	checksumIP(data);
	data[23] = 0x06;
	data[24] = 0x40;
	data[25] = 0x00;
	data[26] = 0x00;
	data[27] = 0x12;
	data[28] = 0x00;
	data[29] = 0x21;
	data[30] = 0x00;
	data[31] = 0x00;
	data[32] = 0x45;


}

void checksumIP(uint8_t data[])
{
	uint16_t checksum;
	uint16_t array[9] = {((data[32]<<8) | data[31]),
			 ((data[30]<<8) | data[29]),
			 ((data[28]<<8) | data[27]),
			 ((data[26]<<8) | data[25]),
			 ((data[24]<<8) | data[23]),
			 ((data[20]<<8) | data[19]),
			 ((data[18]<<8) | data[17]),
			 ((data[16]<<8) | data[15]),
			 ((data[14]<<8) | data[13])};
	checksum = 0xFFFF - (array[0] + array[1] + array[2] + array[3] + array[4] + array[5] + array[6] + array[7] + array[8]);

	data[21] = checksum;
}

int main(void)
{
	init_printf();
	init_serial(9600);
	ad_init(0x00);
	ad_init(0x01);
	ad_init(0x02);
	uint8_t gyro_X,gyro_Y,gyro_Z,temp,id;
	temp = 0;
	id = 0;
	uint8_t data[33];	

	while (1)
	{
		gyro_X = ad_sample(0x00);
		gyro_Y = ad_sample(0x01);
		gyro_Z = ad_sample(0x02);
		id = 0x00 | (checkParity(gyro_X) << 3) | (checkParity(gyro_Y) << 2) | (checkParity(gyro_Z) << 1) | (checkParity(temp));
		datagrammeIP(data, gyro_X, gyro_Y, gyro_Z, temp, id);
	}

	return 0;
}
