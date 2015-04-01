/** Functions for serial port **/

#include <avr/io.h>
#include <stdio.h>

#include "serial.h"

static int send_serial_printf(char c,FILE *stream);
static FILE mystdout=FDEV_SETUP_STREAM(send_serial_printf,NULL,_FDEV_SETUP_WRITE);

void init_serial(int speed){
/* Set baud rate */
UBRR0 = F_CPU/(((unsigned long int)speed)<<4)-1;

/* Enable transmitter & receiver */
UCSR0B = (1<<TXEN0 | 1<<RXEN0);

/* Set 8 bits character and 1 stop bit */
UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);

/* Set off UART baud doubler */
UCSR0A &= ~(1 << U2X0);
}

void send_serial(char c){
loop_until_bit_is_set(UCSR0A, UDRE0);
UDR0 = c;
}

char get_serial(void){
loop_until_bit_is_set(UCSR0A, RXC0);
return UDR0;
}

void init_printf(void){
init_serial(9600);
stdout=&mystdout;
}

static int send_serial_printf(char c,FILE *stream){
if(c=='\n') send_serial('\r');
send_serial(c);
return 0;
}
