#include "twi.h"
#include <stdio.h>

#define STATUS_START_SENT	0x08
#define STATUS_RESTART_SENT	0x10
#define STATUS_ACK_ADDR		0x18
#define STATUS_NO_ACK_ADDR	0x20
#define STATUS_ACK_DATA		0x28
#define STATUS_NO_ACK_DATA	0x30
#define STATUS_BUS_FAILED	0x38

#define STATUS_ACK_ADDR_R	0x40
#define STATUS_NO_ACK_ADDR_R	0x48
#define STATUS_REC_DATA_ACK	0x50
#define STATUS_REC_DATA_NO_ACK	0x58


int twi_writebyte(unsigned char *buff, unsigned char addr_dev, unsigned char offset, unsigned int num)
{
	int ret = -1;
	unsigned char status = 0;
	int i = 0;

	status = twi_start();
	if(status != STATUS_START_SENT) {
		printf("start bus failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	status = twi_write_data((addr_dev<<1) & 0xfe);
	if(status != STATUS_ACK_ADDR) {
		printf("send device address failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	status = twi_write_data(offset);
	if(status != STATUS_ACK_DATA) {
		printf("send data failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}

	for(i=0;i<num;i++) {
		status = twi_write_data(buff[i]);
		if(status != STATUS_ACK_DATA) {
			printf("send data failed status=0x%x\n",status);
			ret = -1;
			goto out1;
		}
	}

	ret = 0;
out1:
	twi_stop();

	return ret;
}

int twi_readbyte(unsigned char *p_dst, unsigned char addr_dev, unsigned char offset, unsigned int num)
{
	int ret = -1;
	unsigned char status = 0;
	int i = 0;
	int not_ack = 0;

	status = twi_start();
	if(status != STATUS_START_SENT) {
		printf("start bus failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	status = twi_write_data((addr_dev<<1) & 0xfe);
	if(status != STATUS_ACK_ADDR) {
		printf("send device address failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	status = twi_write_data(offset);
	if(status != STATUS_ACK_DATA) {
		printf("send data failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}

	status = twi_start();
	if(status != STATUS_RESTART_SENT) {
		printf("start bus failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	status = twi_write_data((addr_dev<<1) | 0x01);
	if(status != STATUS_ACK_ADDR_R) {
		printf("send device address failed status=0x%x\n",status);
		ret = -1;
		goto out1;
	}
	for(i=0;i<num;i++) {
		/*
		 * when burst read, one byte one ack,
		 * till last byte, not ack and stop bus
		 */
		not_ack = (i<(num-1))? 0:1;
		*(p_dst+i) = twi_read_data(not_ack);
		if(not_ack) {
			if((TWSR & 0xF8) != STATUS_REC_DATA_NO_ACK) {
				printf("recive data ,ack! \n");
			}
		}
		else {
			if((TWSR & 0xF8) != STATUS_REC_DATA_ACK) {
				printf("recive data ,no ack! \n");
			}
		}
	}

	ret = 0;
out1:
	twi_stop();

	return ret;
}


void twi_init(void)
{
#if 0
	//12M/(16 + 2xTWBR x 4^(TWSR & 0x03)) = 12M/(16 + 2x52x4^0)= 100K
	TWCR= 0x00; //disable twi
	TWBR= (1<<5) | (1<<4) | (1<<2); //set bit rate
	TWSR= 0x00; //set prescale
	TWAR= 0x00; //set slave address
	TWCR= (1<<TWEN); //enable twi
#else

	TWCR= 0x00; //disable twi
	TWBR= (1<<6) | (1<<5) | (1<<2); //set bit rate
	TWSR= 0x00; //set prescale
	TWAR= 0x00; //set slave address
	TWCR= (1<<TWEN); //enable twi
#endif
}

void twi_wait_complete(void)
{
	int i = 0;		//time out variable
	// wait for i2c interface to complete operation
	while ((!(TWCR & (1<<TWINT))) && (i < 90))
		i++;
}

unsigned char twi_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
        twi_wait_complete();
	return (TWSR & 0xF8);
}

unsigned char twi_write_data(unsigned char data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
        twi_wait_complete();
	return(TWSR&0b11111000);
}

unsigned char twi_read_data(int not_ack)
{
	if(not_ack)
		TWCR= (1<<TWINT) | (1<<TWEN);
	else
		TWCR= (1<<TWINT) | (1<<TWEA) | (1<<TWEN);

        twi_wait_complete();
	return(TWDR);
}

void twi_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
        twi_wait_complete();
	//	TWCR &= ~(1<<TWEN);
	//	DDRC |= ((1<<PC4)|(1<<PC5));
	//	PORTC |= ((1<<PC4)|(1<<PC5)); //SDL SDA
}

#if 0
void wt24c(unsigned char *p_rsc, unsigned int ad_dst, unsigned int num)
{
	unsigned int n;

	n=ad_dst/PAGE_SIZE;
	if(n) n=(unsigned long)PAGE_SIZE*(n+1)-ad_dst;
	else n=PAGE_SIZE-ad_dst;

	if(n>=num)
	{
		wt24c_fc(p_rsc, ad_dst, num);
		if(syserr!=0) return;
	}
	else
	{
		p_rsc=wt24c_fc(p_rsc, ad_dst, n);
		if(syserr!=0) return;

		num-=n;
		ad_dst+=n;

		while(num>=PAGE_SIZE)
		{
			p_rsc=wt24c_fc(p_rsc, ad_dst, PAGE_SIZE);
			if(syserr!=0) return;

			num-=PAGE_SIZE;
			ad_dst+=PAGE_SIZE;
		}

		if(num)
			wt24c_fc(p_rsc, ad_dst, num);
	}
}

void twi_readbyte(unsigned char *p_dst, unsigned char addr_dev, unsigned char offset, unsigned int num)
{
	unsigned char t=0;
	unsigned char ret=0;

	twi_start();

	ret = twi_write_data((addr_dev<<1) | 0x01);
	if(ret==0x40) {
		twi_write_data(offset);

		for(;num>0;num--) {
			*p_dst=twi_read_data();
			p_dst++;
		}
	}
	else {
		printf("write dev address failed, ret=%x\n",ret);
	}

	twi_stop();
}
#endif
