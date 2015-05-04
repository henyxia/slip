#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 	
#include <stdio.h>
#include <stdbool.h>

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

#define PRESCALER	1024
#define TIME_SLOT 12
#define NB_TICK	312
#define BAUDRATE	103 // UBRR value for 9600

#define NB_TASK 3

#define portSAVE_CONTEXT()  \
	asm volatile ( \
		"push	r0				\n\t" \
		"in	r0, __SREG__			\n\t" \
		"push	r0				\n\t" \
		"push	r1				\n\t" \
		"clr	r1				\n\t" \
		"push 	r2				\n\t" \
		"push	r3				\n\t" \
		"push	r4				\n\t" \
		"push	r5				\n\t" \
		"push	r6				\n\t" \
		"push	r7				\n\t" \
		"push	r8				\n\t" \
		"push	r9				\n\t" \
		"push	r10				\n\t" \
		"push	r11				\n\t" \
		"push	r12				\n\t" \
		"push	r13				\n\t" \
		"push	r14				\n\t" \
		"push	r15				\n\t" \
		"push	r16				\n\t" \
		"push	r17				\n\t" \
		"push	r18				\n\t" \
		"push	r19				\n\t" \
		"push	r20				\n\t" \
		"push	r21				\n\t" \
		"push	r22				\n\t" \
		"push	r23				\n\t" \
		"push	r24				\n\t" \
		"push	r25				\n\t" \
		"push	r26				\n\t" \
		"push	r27				\n\t" \
		"push	r28				\n\t" \
		"push	r29				\n\t" \
		"push	r30				\n\t" \
		"push	r31				\n\t");


#define portRESTORE_CONTEXT() \
	asm volatile ( \
		"pop	r31				\n\t" \
		"pop	r30				\n\t" \
		"pop	r29				\n\t" \
		"pop	r28				\n\t" \
		"pop	r27				\n\t" \
		"pop	r26				\n\t" \
		"pop	r25				\n\t" \
		"pop	r24				\n\t" \
		"pop	r23				\n\t" \
		"pop	r22				\n\t" \
		"pop	r21				\n\t" \
		"pop	r20				\n\t" \
		"pop	r19				\n\t" \
		"pop	r18				\n\t" \
		"pop	r17				\n\t" \
		"pop	r16				\n\t" \
		"pop	r15				\n\t" \
		"pop	r14				\n\t" \
		"pop	r13				\n\t" \
		"pop	r12				\n\t" \
		"pop	r11				\n\t" \
		"pop	r10				\n\t" \
		"pop	r9				\n\t" \
		"pop	r8				\n\t" \
		"pop	r7				\n\t" \
		"pop	r6				\n\t" \
		"pop	r5				\n\t" \
		"pop	r4				\n\t" \
		"pop	r3				\n\t" \
		"pop	r2				\n\t" \
		"pop	r1				\n\t" \
		"pop	r0				\n\t" \
		"out	__SREG__, r0			\n\t" \
		"pop	r0				\n\t");


typedef struct Tache {
	int adresse;
	bool active;
	void (*fonction)(void);
} Tache;

uint8_t data[32];
int period=100; //x10-2 seconds

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

int receive_packet()
{
   /* RECV_PACKET: receives a packet into the buffer located at "p".
    *      If more than len bytes are received, the packet will
    *      be truncated.
    *      Returns the number of bytes stored in the buffer.
    */

   uint8_t c;

   /* sit in a loop reading bytes until we put together
    * a whole packet.
    * Make sure not to copy them into the packet if we
    * run out of room.
    */
		while(1)
		{
           // get a character to process
           c = get_serial();

           // handle bytestuffing if necessary
           switch(c) {

           /* if it's an END character then we're done with
            * the packet
            */
           case END:
                   return -1;

           /* if it's the same code as an ESC character, wait
            * and get another character and then figure out
            * what to store in the packet based on that.
            */
           case ESC:
                   c = get_serial();

                   /* if "c" is not one of these two, then we
                    * have a protocol violation.  The best bet
                    * seems to be to leave the byte alone and
                    * just stuff it into the packet
                    */
                   switch(c) {
                   case ESC_END:
                           c = END;
                           return c;
                   case ESC_ESC:
                           c = ESC;
                           return c;
                   }
			default:
					return c;
                           
                   
           }
		}
}

void datagrammeIP(uint8_t data [],int x, int y, int z, int t)
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
	data[12] = 0xAC; // 172
	data[13] = 0x1A; // 26
	data[14] = 0x4F; // 79
	data[15] = 0xC9; // 201
	data[16] = 0xAC; // 172
	data[17] = 0x1A; // 26
	data[18] = 0x4F; // 79
	data[19] = 0xFF; // 255
	//Checksum
	uint32_t tempChecksum = 0;
	tempChecksum += (((uint32_t)data[0]) << 8) + data[1];
	tempChecksum += (((uint32_t)data[2]) << 8) + data[3];
	tempChecksum += (((uint32_t)data[4]) << 8) + data[5];
	tempChecksum += (((uint32_t)data[6]) << 8) + data[7];
	tempChecksum += (((uint32_t)data[8]) << 8) + data[9];
	tempChecksum += (((uint32_t)data[12]) << 8) + data[13];
	tempChecksum += (((uint32_t)data[14]) << 8) + data[15];
	tempChecksum += (((uint32_t)data[16]) << 8) + data[17];
	tempChecksum += (((uint32_t)data[18]) << 8) + data[19];

	tempChecksum = (tempChecksum & 0x0000FFFF) + ((tempChecksum & 0xFFFF0000) >> 16);
	if (tempChecksum > 0xFFFF)
		tempChecksum = (tempChecksum & 0x0000FFFF) + ((tempChecksum & 0xFFFF0000) >> 16);
	tempChecksum = 0xFFFF - tempChecksum;
	data[10] = (tempChecksum & 0x0000FF00) >> 8;
	data[11] = (tempChecksum & 0x000000FF);


	//UDP
	data[20] = 0xA4; // Src port 42001
	data[21] = 0x11; // Src port 42001
	data[22] = 0x30; // Dest port 12345
	data[23] = 0x39; // Dest port 12345
	data[24] = 0x00; // Length 
	data[25] = 0x0D; // Length

	//Data	
	data[29] = x; // X
	data[30] = y; // Y
	data[31] = z; // Z
	data[32] = t; // T
	data[28] = 0x00 | (checkParity(x) << 3) | (checkParity(y) << 2) | (checkParity(z) << 1) | (checkParity(t));

	tempChecksum = 0;
	tempChecksum += (((uint32_t)data[12]) << 8) + data[13]; // IP Src
	tempChecksum += (((uint32_t)data[14]) << 8) + data[15]; // IP Src
	tempChecksum += (((uint32_t)data[16]) << 8) + data[17]; // IP Dest
	tempChecksum += (((uint32_t)data[18]) << 8) + data[19]; // IP Dest
	tempChecksum += data[9]; // Protocol
	tempChecksum += (((uint32_t)data[24]) << 8) + data[25]; // UDP Length
	tempChecksum += (((uint32_t)data[20]) << 8) + data[21]; // Source Port
	tempChecksum += (((uint32_t)data[22]) << 8) + data[23];	// Dest Port
	tempChecksum += (((uint32_t)data[24]) << 8) + data[25]; //Length
	tempChecksum += (((uint32_t)data[28]) << 8) + data[29]; //Data
	tempChecksum += (((uint32_t)data[30]) << 8) + data[31]; //Data
	tempChecksum += (((uint32_t)data[32]) << 8);			//Data

	tempChecksum = (tempChecksum & 0x0000FFFF) + ((tempChecksum & 0xFFFF0000) >> 16);
	if (tempChecksum > 0xFFFF)
		tempChecksum = (tempChecksum & 0x0000FFFF) + ((tempChecksum & 0xFFFF0000) >> 16);
	tempChecksum = ~tempChecksum;
	data[26] = (tempChecksum & 0x0000FF00) >> 8;	// Checksum UDP
	data[27] = (tempChecksum & 0x000000FF);			// Checksum UDP
}

void send_data()
{

	uint8_t gyro_X,gyro_Y,gyro_Z,temp;
	float Vout;
	temp = 0;

	while(1)
	{
		/*ad_init(0x00);
		gyro_X = ad_sample();
		ad_init(0x01);
		gyro_Y = ad_sample();
		ad_init(0x02);
		gyro_Z = ad_sample();
		ad_init(0x04);
		temp = ad_sample();
		Vout = temp*0.01953;
		temp = (int)((Vout-0.5)*100);
		datagrammeIP(data, gyro_X, gyro_Y, gyro_Z, temp);

		int i;
		for(i=0; i<33; i++)
		{
			send_packet(data[i]);		
		}
		send_serial(END);
		_delay_ms(200); //wait 1s between the two packets*/
	}
}

void receive_data()
{
	uint8_t rec_data[32];
	char c=0x00;
	int i=1;
	while(1)
	{
		get_serial();
		cli();
		c = receive_packet();
		send_serial(c);
		rec_data[0] = c;
		while (c !=-1)
		{
			c = receive_packet();
			if(c != -1)
			{
				rec_data[i] = c;
				send_serial(c);
				i++;
			}
		}
		
		for(i=0; i<32; i++)
		{
			//send_serial(rec_data[i]);
		}
		sei();
	}

}

void process_request()
{
	/*//TODO test parity
	if((rec_data[28] & 0xf0) == 0x00))
	{
		
	}*/
}

void delay_cs(int time)
{
	int i;
	for(i=0;i<time;i++)
	{
		_delay_ms(10);
	}
}

void init_task_led(void)
{
	DDRB = 0x10;
}

void blink_led()
{
	int period_cs;
	while(1)
	{
		period_cs = (period)/10;
		PORTB &= 0xef;	//Blinking the LED at 1Hz
		delay_cs(period_cs);
		PORTB |= 0x10;
		delay_cs(period_cs);
	}
}

Tache taches[NB_TASK] = {
	{0x500,0,blink_led},
	{0x700,0,send_data},
	{0x600,0,receive_data}
};

int tache_courante = 0;

void scheduler()
{
	tache_courante++;
	tache_courante = tache_courante % NB_TASK;
}

void init_timer()
{
	TCCR1B |= _BV(WGM12); // CTC mode with value in OCR1A 
	TCCR1B |= _BV(CS12);  // CS12 = 1; CS11 = 0; CS10 =1 => CLK/1024 prescaler
	TCCR1B |= _BV(CS10);
	OCR1A   = NB_TICK;
	TIMSK1 |= _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	if(taches[tache_courante].active){
		portSAVE_CONTEXT();
		taches[tache_courante].adresse=SP;
	}
	scheduler();
	SP = taches[tache_courante].adresse;
	if(taches[tache_courante].active){
		portRESTORE_CONTEXT();
	} else {
		taches[tache_courante].active = 1;
		sei();
		taches[tache_courante].fonction();
	}
}


int main(void)
{
	init_timer();
	init_printf();
	init_task_led();

	sei();
	while(1)
	{

	}
	cli();
	return 0;
}
