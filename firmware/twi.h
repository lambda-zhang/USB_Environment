#ifndef __TWI_H__
#define __TWI_H__

#include<avr/io.h>

int twi_writebyte(unsigned char *buff, unsigned char addr_dev, unsigned char offset, unsigned int num);
int twi_readbyte(unsigned char *p_dst, unsigned char addr_dev, unsigned char offset, unsigned int num);
void twi_init(void);
unsigned char twi_start(void);
unsigned char twi_write_data(unsigned char data);
unsigned char twi_read_data(int not_ack);
void twi_stop(void);

#endif
