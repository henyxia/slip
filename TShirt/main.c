#include <avr/io.h>
#include <util/delay.h>
#include "main.h"

/* SEND_PACKET: sends a packet of length "len", starting at
 * location "p".
 */
void send_packet(int *p, int len)
{

    /* send an initial END character to flush out any data that may
     * have accumulated in the receiver due to line noise
     */
    send_serial(END);

    /* for each byte in the packet, send the appropriate character
     * sequence
     */
    while(len--) {
    	switch(*p) {
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
            send_serial(*p);
        }
        p++;
    }
    /* tell the receiver that we're done sending the packet
     */
    send_serial(END);
}


void init_serial(int speed)
{
	UBRR0 = CPU_FREQ/(((unsigned long int)speed)<<4)-1;
	UCSR0B = (1<<TXEN0 | 1<<RXEN0);
	UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);
	UCSR0A &= (1 << U2X0);
}

void send_serial(unsigned char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

unsigned char get_serial(void)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

void port_initio(void)
{
	//DDRD &= 0xFC;
	DDRD |= 0x1C;
	/*
	DDRC = 0x00;
	DDRB &= 0xFE;
	PORTD |= 0x3C;
	*/
}

int analogRead(uint8_t pin)
{
        uint8_t low, high;

        if (pin >= 14)
			pin -= 14; // allow for channel or pin numbers
        ADMUX = (0x01 << 6) | (pin & 0x07);
		ADCSRA|=(1<<ADSC);
        while (bit_is_set(ADCSRA, ADSC));
        low  = ADCL;
        high = ADCH;

        return (high << 8) | low;
}

void ad_init(unsigned char channel)   
{
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);   
	ADMUX|=(1<<REFS0)|(1<<ADLAR);
	ADMUX=(ADMUX&0xf0)|channel;   
	ADCSRA|=(1<<ADEN);
}

void datagrammeIP(uint8_t data [],int x, int y, int z, int t, int id)
{
	//Data
	data[0] = t;
	data[1] = z;
	data[2] = y;
	data[3] = x;
	data[4] = id;
	
	//UDP
	//data[5] = 
	//data[6] = 
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
	init_serial(9600);
	port_initio();
	ad_init(0x00);
	ad_init(0x01);
	ad_init(0x02);
	int gyro_X,gyro_Y,gyro_Z,temp,id;
	temp = 0;
	id = 0;
	uint8_t data[33];

	

	while (1)
	{
		gyro_X = analogRead(0x00);
		gyro_Y = analogRead(0x01);
		gyro_Z = analogRead(0x02);
		datagrammeIP(data, gyro_X, gyro_Y, gyro_Z, temp, id);
		checksumIP(data);
	}

	return 0;
}
