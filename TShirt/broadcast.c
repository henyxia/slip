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
#define	END		0xC0
#define	ESC		0xDB
#define	ESC_END		0xDC
#define	ESC_ESC		0xDD

void send_packet(uint8_t p)
{
    /* for each byte in the packet, send the appropriate character
     * sequence
     */
    
   switch(p) {
      /* if it's the same code as an END character, we send a
       * special two character code so as not to make the
       * receiver think we sent an END
       */
  		case END:
          send_serial(ESC);
          send_serial(ESC_END);
          break;

          /* if it's the same code as an ESC character,
           * we send a special two character code so as not
           * to make the receiver think we sent an ESC
           */
          case ESC:
          send_serial(ESC);
          send_serial(ESC_ESC);
          break;
          /* otherwise, we just send the character
           */
          default:
          send_serial(p);
      }
}

void datagrammeIP(uint8_t data [],int x, int y, int z, int t, int id)
{
	//IP
	data[0] = 0x45; // v4, no opts
	data[1] = 0x00; // DSCP / ECN
	data[2] = 0x00; // Length
	data[3] = 0x21; // Length
	data[4] = 0x00; // Id
	data[5] = 0x12; // Id
	data[6] = 0x00; // Flags + Fragment Offset
	data[7] = 0x00; // Fragment Offset
	data[8] = 0x40; // TTL
	data[9] = 0x11; // UDP
	data[10] = 0x00; // IP Checksum
	data[11] = 0x00; // IP Checksum
	checksumIP(data);

	// Original Packet
	data[12] = 0xC0; // 192
	data[13] = 0xA8; // 168
	data[14] = 0x01; // 1
	data[15] = 0x04; // 4
	data[16] = 0xAC; // 172
	data[17] = 0x1A; // 26
	data[18] = 0x4F; // 79
	data[19] = 0xFF; // 255

	//UDP
	data[20] = 0xA4; // Src port 42001
	data[21] = 0x11; // Src port 42001
	data[22] = 0x30; // Dest port 12345
	data[23] = 0x39; // Dest port 12345
	data[24] = 0x00; // Length 
	data[25] = 0x0D; // Length
	data[26] = 0x00; // Checksum UDP
	data[27] = 0x00; // Checksum UDP

	//Data
	data[28] = 0x01; // ID
	data[29] = 0x02; // X
	data[30] = 0x03; // Y
	data[31] = 0x04; // Z
	data[32] = 0x05; // T
}

void checksumIP(uint8_t data[])
{
	uint16_t checksum;
	uint16_t array[9] = {((data[0]<<8) | data[1]),
			 ((data[2]<<8) | data[3]),
			 ((data[4]<<8) | data[5]),
			 ((data[6]<<8) | data[7]),
			 ((data[8]<<8) | data[9]),
			 ((data[12]<<8) | data[13]),
			 ((data[14]<<8) | data[15]),
			 ((data[16]<<8) | data[17]),
			 ((data[18]<<8) | data[19])};
	checksum = 0xFFFF - (array[0] + array[1] + array[2] + array[3] + array[4] + array[5] + array[6] + array[7] + array[8]);
	uint16_t test = checksum + array[0] + array[1] + array[2] + array[3] + array[4] + array[5] + array[6] + array[7] + array[8];

	//send_serial(test);
	//send_serial((test >> 8));

	data[10] = (checksum >> 4) & 0x0F;
	data[11] = checksum & 0x0F;
}

int main(void)
{
	init_printf();
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

		int i;
		for(i=0; i<33; i++)
		{
			send_packet(data[i]);
			
		}
		/* tell the receiver that we're done sending the packet
		 */
		send_serial(END);

		_delay_ms(1000); //wait 1s between the two packets
	}
	return 0;
}
