#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#ifndef _TWI_LIB_H_
#define _TWI_LIB_H_ 1
//! Set the I2C transaction bitrate (in KHz)
void i2cSetBitrate(uint16_t bitratekHz);
//! Send an I2C start condition in Master mode or repeated start condition
void i2cSendStart(void);
//! Send an I2C stop condition in Master mode
uint8_t i2cSendStop(void);
//! Send an (address|R/W) combination or a data byte over I2C
void i2cSendByte(unsigned char data);
//! Receive a data byte over I2C
void i2cReceiveByteACK(void);
//! Receive a data byte over I2C
void i2cReceiveByteNACK(void);
// get received byte back
uint8_t i2cGetReceivedByte(void);
//! Wait for current I2C operation to complete
uint8_t i2cWaitForComplete(void);
// Check for expected error code
uint8_t checki2cReturnCode(uint8_t expected_return_code);
#endif /* _TWI_LIB_H_ */
