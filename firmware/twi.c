#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

void i2cSetBitrate(uint16_t bitratekHz)
{
	uint8_t bitrate_div;
	// set i2c bitrate
	// Set Prescaler to one --> TWPS = 1
	TWSR =((0<<TWPS0)|(0<<TWPS1));
	//calculate bitrate division
	bitrate_div = (((float)F_CPU/1000.0)/(float)bitratekHz);
	if(bitrate_div >= 16){
		bitrate_div = (float)(bitrate_div-16)/8;
	}
	//printf("DIV: %d \n",bitrate_div);
	TWBR=(uint8_t)bitrate_div;
	//TWBR=0; -> max speed
}

void i2cSendStart(void)
{
	// send start condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}

uint8_t i2cSendStop(void)
{

	uint8_t i = 0;		//time out variable
	uint8_t error_code=0;
	// transmit stop condition
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

	// wait until stop condition is executed and bus released
	while ((TWCR & (1<<TWSTO)) && (i < 90)){
		_delay_us(1);
		i++;
	}
	if (i>89){
		_delay_us(1);
		error_code=100;

	}
	return error_code;
}

void i2cSendByte(unsigned char data)
{
	// save data to the TWDR
	TWDR = data;
	// begin send
	TWCR = (1<<TWINT)|(1<<TWEN);
}

void i2cReceiveByteACK(void)
{

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
}

void i2cReceiveByteNACK(void)
{

	TWCR = (1<<TWINT)|(1<<TWEN);
}

uint8_t i2cWaitForComplete(void)
{
	uint8_t i = 0;		//time out variable
	uint8_t error_code=0;

	// wait for i2c interface to complete operation
	while ((!(TWCR & (1<<TWINT))) && (i < 90)){
		_delay_us(1);
		i++;
	}
	if (i>89){
		_delay_us(1);
		error_code=100;

	}
	return error_code;
}

uint8_t i2cGetReceivedByte(void)
{
	// retieve received data byte from i2c TWDR
	return (uint8_t)TWDR;
}

uint8_t checki2cReturnCode(uint8_t expected_return_code){
	uint8_t error_code=0;
	uint8_t i=0;
	//while (error_code!=0 && i!=0 && i<3){
	if ((TWSR & 0xF8) != expected_return_code){
		error_code=10;
	}
	i++;
	//	}
	return error_code;
}
