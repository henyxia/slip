/** Functions for the analog converter **/

void ad_init(unsigned char channel)   
{   
ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);   
ADMUX|=(1<<REFS0)|(1<<ADLAR);
ADMUX=(ADMUX&0xf0)|channel;   
ADCSRA|=(1<<ADEN);
}   

unsigned int ad_sample(void){
ADCSRA|=(1<<ADSC);
while(bit_is_set(ADCSRA, ADSC));
return ADCH;
}
