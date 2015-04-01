/** Functions for the analog converter **/

#include <avr/io.h>
#include <stdio.h>

#include "analog.h"

void ad_init(unsigned char channel)   
{   
ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);   
ADMUX|=(1<<REFS0)|(1<<ADLAR);
ADMUX=(ADMUX&0xf0)|channel;   
ADCSRA|=(1<<ADEN);
}   

unsigned int ad_sample(uint8_t pin){
ADMUX = (0x01 << 6) | (pin & 0x07);
ADCSRA|=(1<<ADSC);
while(bit_is_set(ADCSRA, ADSC));
return ADCH;
}
