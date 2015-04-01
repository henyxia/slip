#ifndef __MAIN_H__
#define	__MAIN_H__

#define CPU_FREQ    16000000L
#define	VAL25		895
#define	VAL105		246
#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

void send_packet(int *p, int len);
void init_serial(int speed);
void send_serial(unsigned char c);
unsigned char get_serial(void);
void port_initio(void);
int analogRead(uint8_t pin);
void ad_init(unsigned char channel);
void datagrammeIP(uint8_t data [],int x, int y, int z, int t, int id);
void checksumIP(uint8_t data[]);

#endif
